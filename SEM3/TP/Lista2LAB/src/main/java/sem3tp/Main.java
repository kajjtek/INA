package sem3tp;

import java.util.ArrayList;
import java.util.Scanner;

public class Main {
    public ArrayList<Book> bookArrayList = new ArrayList<>();
    public ArrayList<User> userArrayList = new ArrayList<>();

    public static void main(String[] args) {
        Library library = new Library();
        Controller controller = new Controller(library);
        controller.start();
    }
}