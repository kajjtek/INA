package sem3tp;

public interface Storage<T> {
    public void add(String name);
    public T findByName(String name);
    public void findAll();
}
