package sem3tp;
//Klasa odpowiada za interfejs konsolowy i wzywanie odpowiednich funkcji
import java.util.Scanner;

public class Controller {
//    public Library library;
    public BookStorage bookStorage;
    public UserStorage userStorage;

    Controller(){
//        this.library=library;
        this.bookStorage=new BookStorage();
        this.userStorage=new UserStorage();
    }

    public void start(){
        welcomeScreen();
    }

    public void welcomeScreen(){
        System.out.println("Welcome in online library!\n Choose what you want to do:\n");
        System.out.println("""
                1. Show all users
                2. Show all books
                3. Show the user of the given name
                4. Show the book of the given name
                5. Create an user
                6. Create a book
                7. Create a copy
                8. Borrow a copy
                9. Show a copy""");
        Scanner scanner = new Scanner(System.in);
        try{
            switch (Integer.parseInt(scanner.nextLine())) {
                case 1:
                    userStorage.findAll();
                    break;
                case 2:
                    bookStorage.findAll();
                    break;
                case 3:
                    System.out.println("Input the name of the user you are interested in finding");
                    try {
                        User temp = userStorage.findByName(scanner.nextLine());
                        if(temp==null)throw new Exception("User not found");
                        temp.print();
                    } catch (Exception e) {
                        System.out.println(e.getMessage());
                    }
                    break;
                case 4:
                    System.out.println("Input the name of the book you are interested in finding");
                    try{
                        Book temp = bookStorage.findByName(scanner.nextLine());
                        if (temp == null) {
                            throw new Exception("The book does not exist");
                        }
                        temp.print();
                    } catch (Exception e) {
                        System.out.println(e.getMessage());
                    }
                    break;
                case 5:
                    System.out.println("Input the name of the user you are interested in creating");
                    userStorage.add(scanner.nextLine());
                    break;
                case 6:
                    System.out.println("Input the name of the book you are interested in creating");
                    bookStorage.add(scanner.nextLine());
                    break;
                case 7:
                    System.out.println("Input the name of the book copy of which you are interested in creating");
                    try {
                        Book temp = bookStorage.findByName(scanner.nextLine());
                        if(temp==null)throw new Exception("Book not found");
                        temp.createCopy();
                    }catch (Exception e){
                        System.out.println(e.getMessage());
                    }
                    break;
                case 8:
                    System.out.println("Input the name of the book you are interested in borrowing");
                    String bookName = scanner.nextLine();
                    System.out.println("Input the user that borrows the copy");
                    String username = scanner.nextLine();
                    try {
                        User tempUser = userStorage.findByName(username);
                        if(tempUser==null)throw new Exception("User not found");
                        Book temp = bookStorage.findByName(bookName);
                        if(temp==null)throw new Exception("Book not found");
                        temp.borrow(tempUser);
                    } catch (Exception e) {
                        System.out.println(e.getMessage());
                    }
                    break;
                case 9:
                    System.out.println("Input the name of the book copy of which you are interested in finding");
                    String copyName = scanner.nextLine();
                    System.out.println("Input the id of the book copy of which you are interested in finding");
                    int copyId = Integer.parseInt(scanner.nextLine());
                    try{
                        Book temp = bookStorage.findByName(copyName);
                        if (temp == null) {
                            throw new Exception("The book does not exist");
                        }
                        temp.showCopy(copyId);
                    } catch (Exception e) {
                        System.out.println(e.getMessage());
                    }
                    break;
                default:
                    throw new Exception("Input a number between 1 and 9");
            }
            System.out.println("Click enter to continue, type exit to exit");
            String line = scanner.nextLine();
            if("exit".equals(line))System.exit(0);
            welcomeScreen();
        }catch(Exception e){
            System.out.println(e.getMessage());
        }
    }
}
