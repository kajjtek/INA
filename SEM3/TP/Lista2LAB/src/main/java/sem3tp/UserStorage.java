package sem3tp;

import java.util.ArrayList;

public class UserStorage implements Storage<User>{
    private final ArrayList<User> userArrayList;

    UserStorage(){
        this.userArrayList = new ArrayList<>();
    }

    @Override
    public void add(String name){
        userArrayList.add(new User(userArrayList.size(),name));
    }

    @Override
    public User findByName(String name){
        for(User user:userArrayList){
            if(user.username.equals(name)){
                return user;
            }
        }
        return null;
    }

    @Override
    public void findAll() {
        for(User user:userArrayList){
            user.print();
        }
    }
}