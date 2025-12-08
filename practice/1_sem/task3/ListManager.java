
public class ListManager {
    private String[] data;
    private int len;

    public ListManager() {
        len=0;
        data = new String[len];
    }

    public void addElement(String el) {
        String[] new_data = new String[len + 1];
        int i = 0;
        for (; i < len; i++) {
            if (data[i].length() < el.length())
                new_data[i] = data[i];
            else
                break;
        }
        new_data[i]=el;
        for (; i < len; i++) {
            new_data[i+1]=data[i];
        }
        len++;
        data=new_data;

    }

    public String getMaxString() {
        return data[len-1];
    }

    public float getMeanLen() {
        float sum_len = 0;
        for (int i = 0; i < len; i++)
            sum_len += data[i].length();
        return sum_len / len;
    }

    public void printData() {
        // Проверяем, не null ли массив
        if (data == null) {
            System.out.println("Массив данных пуст");
            return;
        }
        
        // Используем цикл для перебора элементов
        for (int i = 0; i < data.length; i++) {
            System.out.println("Элемент " + i + ": " + data[i]);
        }
    }
}
