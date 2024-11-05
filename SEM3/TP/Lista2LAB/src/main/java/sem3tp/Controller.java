package sem3tp;

import java.util.Scanner;

public class Controller {
    public Library library;

    Controller(Library library){
        this.library=library;
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
                    library.showAllUsers();
                    break;
                case 2:
                    library.showAllBooks();
                    break;
                case 3:
                    System.out.println("Input the name of the user you are interested in finding");
                    library.userStorage.findByName(scanner.nextLine());
                    library.showUser(scanner.nextLine());
                    break;
                case 4:
                    System.out.println("Input the name of the book you are interested in finding");
                    library.showBook(scanner.nextLine());
                    break;
                case 5:
                    System.out.println("Input the name of the user you are interested in creating");
                    library.createUser(scanner.nextLine());
                    break;
                case 6:
                    System.out.println("Input the name of the book you are interested in creating");
                    library.createBook(scanner.nextLine());
                    break;
                case 7:
                    System.out.println("Input the name of the book copy of which you are interested in creating");
                    library.createCopy(scanner.nextLine());
                    break;
                case 8:
                    System.out.println("Input the name of the book you are interested in borrowing");
                    String bookName = scanner.nextLine();
                    System.out.println("Input the user that borrows the copy");
                    String username = scanner.nextLine();
                    library.borrowCopy(bookName, username);
                    break;
                case 9:
                    System.out.println("Input the name of the book copy of which you are interested in finding");
                    String copyName = scanner.nextLine();
                    System.out.println("Input the id of the book copy of which you are interested in finding");
                    int copyId = Integer.parseInt(scanner.nextLine());
                    library.showCopy(copyName, copyId);
                    break;
                default:
                    throw new Exception("Input a number between 1 and 9");
            }
            System.out.println("Click enter to continue");
            scanner.nextLine();
            welcomeScreen();
        }catch(Exception e){
            System.out.println(e.toString());
            return;
        }
    }
}
