package sem3tp.Storage;

import sem3tp.Poles.Pole;

import java.util.ArrayList;

public class PoleStorage extends  Storage<Pole>{
    public PoleStorage(){
        this.list=new ArrayList<Pole>();
    }

    @Override
    public boolean contains(Pole o) {
        return super.contains(o);
    }

    @Override
    public int getSize() {
        return super.getSize();
    }

    @Override
    public void insert(Pole pole) {
        super.insert(pole);
    }

    @Override
    public Pole get(Pole pole) {
        return super.get(pole);
    }

    @Override
    public void delete(Pole pole) {
        super.delete(pole);
    }

    @Override
    public void update(Pole pole) {
        super.update(pole);
    }
}
