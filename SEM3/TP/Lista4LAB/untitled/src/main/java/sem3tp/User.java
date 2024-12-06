package sem3tp;

public class User {
    private String username;
    private String password;
    //BoardStorage maybeś

    public User(String username, String pwd){
        this.setUsername(username);
        this.setPassword(pwd);
    }

    public String getUsername() {
        return username;
    }

    public void setUsername(String username) {
        this.username = username;
    }


    public void setPassword(String password) {
        this.password = password;
    }
}
