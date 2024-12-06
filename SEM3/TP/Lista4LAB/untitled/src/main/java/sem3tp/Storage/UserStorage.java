package sem3tp.Storage;

import sem3tp.User;

import java.util.ArrayList;

public class UserStorage extends Storage<User>{

    public UserStorage(){
        this.list=new ArrayList<User>();
    }

    @Override
    public void update(User user) {
        super.update(user);
    }
}
