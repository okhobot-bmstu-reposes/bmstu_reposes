template <typename T1, typename T2>
class Pair //???
{
public:
    Pair() {}
    Pair(T1 key, T2 val) : key(key), val(val){};
    void set_key(T1 a_key)
    {
        key = a_key;
    }
    void set_vol(T2 a_val)
    {
        val = a_val;
    }
    T1 get_key()
    {
        return key;
    }
    T2 get_val()
    {
        return val;
    }

private:
    T1 key;
    T2 val;
};
