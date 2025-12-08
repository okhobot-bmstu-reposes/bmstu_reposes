import Math;

public class Triangle {  
    private float a,b,c;
    public Triangle(float a, float b, float c) {  
        this.a=a;
        this.b=b;
        this.c=c;
    }  
    public float getSquare()
    {
        float p=getPerimeter();
        return (float)Math.sqrt(p * (p - a) * (p - b) * (p - c));
    }
    public float getPerimeter()
    {
        return a+b+c;
    }
}  
