package sem3tp.Storage;

import sem3tp.User;

import java.util.ArrayList;

public abstract class Storage<T> implements Storagable<T> {
    ArrayList<T> list;

    @Override
    public T get(T t) {
        return list.get(list.indexOf(t));
    }

    @Override
    public T getByIndex(int index) {
        return list.get(index);
    }

    @Override
    public boolean contains(T o) {
        return list.contains(o);
    }

    @Override
    public void insert(T t) {
        list.add(t);
    }
/*
comparable to be considered
 */
    @Override
    public void delete(T t) {
        list.remove(t);
    }

    @Override
    public void update(T t) {
        //add implementation
    }

    @Override
    public int getSize() {
        return this.list.size();
    }
}
