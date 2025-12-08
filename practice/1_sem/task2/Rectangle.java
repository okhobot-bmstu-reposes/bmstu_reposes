public class Rectangle {  
    private float a,b;
    public Rectangle(float a, float b) {  
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
