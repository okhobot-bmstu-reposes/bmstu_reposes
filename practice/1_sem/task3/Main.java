
public class Main {  
    public static void main(String[] args) {  
        ListManager lmgr=new ListManager();
        lmgr.addElement("rhz");
        lmgr.addElement("hi");
        lmgr.addElement("aaa");
        lmgr.addElement("ooooo");
        lmgr.addElement("й");
        lmgr.printData();

        System.out.println(lmgr.getMaxString()); 
        System.out.println(lmgr.getMeanLen());   
    }  
}  
