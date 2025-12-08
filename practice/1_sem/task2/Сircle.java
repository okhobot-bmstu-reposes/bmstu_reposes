public class Сircle {  
    private final static float PI=3.1415926f;
    private float radius;
    public Сircle(float radius) {  
        this.radius=radius;
    }  
    public float getSquare()
    {
        return PI*radius*radius;
    }
    public float getPerimeter()
    {
        return 2*PI*radius;
    }
}  
