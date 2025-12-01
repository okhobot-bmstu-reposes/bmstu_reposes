import java.util.List;

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
}  
