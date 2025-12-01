import java.util.List;
import java.util.Scanner;
import java.util.ArrayList;
import java.util.Arrays;

public class Main {  
    public static Integer findMax(List<Integer> numbers) {
        if (numbers == null || numbers.isEmpty()) {
            return null;
        }
        
        Integer max = numbers.get(0);
        for (Integer num : numbers) {
            if (num > max) {
                max = num;
            }
        }
        return max;
    }
    public static void main(String[] args) {  
        Scanner scanner = new Scanner(System.in);
        List<Integer> numbers = new ArrayList<>();

        System.out.println("Введите количество чисел: ");  
        Integer n= Integer.parseInt(scanner.nextLine());

        System.out.println("Введите числа(каждое на новой строке):");  

        for(int i=0;i<n;i++)
            numbers.add(Integer.parseInt(scanner.nextLine()));
        
        System.out.println("Максимальное число: "+findMax(numbers));  
    }  
}  
