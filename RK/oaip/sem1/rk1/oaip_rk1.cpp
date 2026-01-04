#include <iostream>
#include <vector>
#include <cstring>
#include <cmath>
#include <fstream>

// Функция ручного преобразования строки в число
double parse_number(const char *str)
{
    double result = 0.0;
    double fraction = 0.0;
    double divisor = 1.0;
    bool negative = false;
    bool afterDot = false;

    // Пропуск пробелов
    while (*str == ' ')
        str++;

    // Проверка знака
    if (*str == '-')
    {
        negative = true;
        str++;
    }
    else if (*str == '+')
    {
        str++;
    }

    // Основной цикл чтения числа
    while (*str != '\0' && *str != ';' && *str != '\n')
    {
        if (*str == '.')
        {
            afterDot = true;
        }
        else if (*str >= '0' && *str <= '9')
        {
            int digit = *str - '0';
            if (!afterDot)
            {
                result = result * 10 + digit;
            }
            else
            {
                divisor *= 10.0;
                fraction += digit / divisor;
            }
        }
        else
        {
            break;
        }
        str++;
    }

    result += fraction;
    if (negative)
        result = -result;
    return result;
}

// Функция извлечения значения по метке
double extract_value(const std::string &line, const char *label)
{
    const char *p = line.c_str();
    const char *found = nullptr;

    // Поиск метки вручную
    while (*p != '\0')
    {
        const char *start = p;
        const char *lab = label;
        while (*start && *lab && *start == *lab)
        {
            start++;
            lab++;
        }
        if (*lab == '\0')
        { // метка найдена
            found = start;
            break;
        }
        p++;
    }

    if (found)
    {
        return parse_number(found);
    }
    else
    {
        return NAN; // если метка не найдена
    }
}

std::string extract_time(std::string line)
{
    // Извлечение времени вручную
    const char *ts = line.c_str();
    const char *pos = nullptr;
    if ((pos = std::strstr(ts, "T:")) != nullptr)
    {
        pos += 2;
        std::string tstamp;
        while (*pos && *pos != ';')
        {
            tstamp.push_back(*pos);
            pos++;
        }
        return tstamp;
    }
    return "";
}

int main()
{
    bool system_stable = true;
    const double nominal_freq = 50, normal_delta = 0.2;
    double mean_power = 0, mean_voltage = 0, max_voltage_delta = 0, freq_delta = 0;
    std::string line;
    std::ifstream inp("input.txt");

    std::vector<std::string> logs, time_stamps;
    std::vector<double> power;
    std::vector<double> voltage, current, freq;

    while (getline(inp, line))
        logs.push_back(line);
    inp.close();

    // выделение места для переменных
    voltage.resize(logs.size());
    current.resize(logs.size());
    freq.resize(logs.size());
    power.resize(logs.size());
    time_stamps.resize(logs.size());

    // Парсинг строк и анализ данных
    for (int i = 0; i < logs.size(); i++)
    {
        // Парсинг строк
        voltage[i] = extract_value(logs[i], "V:");
        current[i] = extract_value(logs[i], "I:");
        freq[i] = extract_value(logs[i], "F:");

        time_stamps[i] = extract_time(logs[i]);

        // Анализ данных
        power[i] = voltage[i] * current[i];
        mean_power += power[i];
        mean_voltage += voltage[i];
    }
    mean_power /= power.size();
    mean_voltage /= voltage.size();

    // Вторичный анализ данных
    for (int i = 0; i < logs.size(); i++)
    {
        double vd = abs(voltage[i] - mean_voltage), fd = abs(freq[i] - nominal_freq);
        max_voltage_delta = std::max(max_voltage_delta, vd);
        if (fd > normal_delta)
        {
            std::cout << "Frequency deviation at T=" << time_stamps[i] << ": " << fd << "Hz" << std::endl;
            if (vd > normal_delta)
                system_stable = false;
        }
    }

    // Вывод результатов
    std::cout << "Average Power: " << mean_power << " W" << std::endl;
    std::cout << "Voltage deviation max: " << max_voltage_delta << " V" << std::endl;
    std::cout << "System stable: " << (system_stable ? "true" : "false") << std::endl;

    return 0;
}
