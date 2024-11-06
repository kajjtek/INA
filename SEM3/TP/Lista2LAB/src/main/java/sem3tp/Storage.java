package sem3tp;

public interface Storage<T> {
    public void add(String name);
    public T findByName(String name);
    public void findAll();
}

//Pozwala na zaimplementowanie bazy danych - mozna stworzyc DatabaseStorage implements Storage<T>
//public void add(){
//      query = Insert into T_table VALUES(?,?,?) db.query itd;
//}
