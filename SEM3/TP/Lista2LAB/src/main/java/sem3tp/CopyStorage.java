package sem3tp;

import java.util.ArrayList;

public class CopyStorage {
    private final ArrayList<Copy> copyArrayList;

    CopyStorage(){
        this.copyArrayList = new ArrayList<>();
    }

    public Copy borrow(){
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
