import java.util.ArrayList;

public class Cart {
    private ArrayList<Product> products;


    public Cart() 
    {
        products=new ArrayList<Product>();
    }

    public void addProduct(Product p)
    {
        products.add(p);
    }
    public void removeProduct(String name)
    {
        products.removeIf(e->e.getName()==name);
    }

    public double getTotal()
    {
        double res=0;
        for(int i=0;i<products.size();i++) 
            res+=products.get(i).getPrice();
        return res;
    }

    public void printCart()
    {
        System.out.println("== Корзина =="); 
        for(int i=0;i<products.size();i++)
            System.out.println((i+1)+". "+products.get(i).toString()); 
        System.out.println("Итого: "+getTotal()); 
    }
    
}
