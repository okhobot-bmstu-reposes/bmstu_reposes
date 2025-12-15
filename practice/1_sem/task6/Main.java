
public class Main {  
    public static void main(String[] args) {  
        Product milk=new Product("Milk", 200); 
        Product cake=new Product("Cake", 500); 
        Product cookies=new Product("Cookies", 270);
        Cart cart=new Cart();

        cart.addProduct(milk);
        cart.addProduct(cake);
        cart.addProduct(cookies);  

        cart.printCart();

        cart.removeProduct("Cake");
        System.out.println("Удален продукт: Cake");

        cart.printCart();
    }  
}  
