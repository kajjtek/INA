package sem3tp;
//Implementacja Storage dla copy
import java.util.ArrayList;

public class CopyStorage {
    private final ArrayList<Copy> copyArrayList;

    CopyStorage(){
        this.copyArrayList = new ArrayList<>();
    }

    public void borrow(User user){
        try {
            Copy temp = this.isBorrowable();
            if(temp==null)throw new Exception("There is no copy that can be borrowed");
            temp.borrow(user);
        }catch (Exception e){
            System.out.println(e.getMessage());
        }
    }

    public Copy isBorrowable(){
        for(Copy copy:copyArrayList){
            if(!copy.isBorrowed){
                return copy;
            }
        }
        return null;
    }

    public void add(int Id){
        copyArrayList.add(new Copy(copyArrayList.size(),Id));
    }

    public Copy findById(int Id){
        for(Copy copy:copyArrayList){
            if(copy.id==Id){
                return copy;
            }
        }
        return null;
    }
}
