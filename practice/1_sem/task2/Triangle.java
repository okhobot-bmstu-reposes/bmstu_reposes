import Math;

public class Triangle {  
    private float a,b,c;
    public Triangle(float a, float b, float c) {  
        if(a<=0 || b<=0 ||c<=0 || a+b<=c || a+c<=b || c+b<=a) throw new RuntimeException("There is no figure with such parameters.");
        this.a=a;
        this.b=b;
        this.c=c;
    }  
    public float getSquare()
    {
        float p=getPerimeter()/2;
        return (float)Math.sqrt(p * (p - a) * (p - b) * (p - c));
    }
    public float getPerimeter()
    {
        return a+b+c;
    }
}  
