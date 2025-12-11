public class Rectangle {  
    private float a,b;
    public Rectangle(float a, float b) {  
        if(a<=0 || b<=0) throw new RuntimeException("There is no figure with such parameters.");
        this.a=a;
        this.b=b;
    }  
    public float getSquare()
    {
        return a*b;
    }
    public float getPerimeter()
    {
        return (a+b)*2;
    }
}  
