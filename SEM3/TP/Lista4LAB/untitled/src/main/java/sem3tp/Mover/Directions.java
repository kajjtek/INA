package sem3tp.Mover;

public enum Directions {
    West{
        @Override
        public int addXCord(){
            return -1;
        }

        @Override
        public int addYCord(){
            return 0;
        }

        @Override
        public int addZCord(){
            return 1;
        }
    },
    East{
        @Override
        public int addXCord(){
            return 1;
        }

        @Override
        public int addYCord(){
            return 0;
        }

        @Override
        public int addZCord(){
            return -1;
        }
    },
    NorthEast{
        @Override
        public int addXCord(){
            return 1;
        }

        @Override
        public int addYCord(){
            return -1;
        }

        @Override
        public int addZCord(){
            return 0;
        }
    },
    NorthWest{
        @Override
        public int addXCord(){
            return 0;
        }

        @Override
        public int addYCord(){
            return -1;
        }

        @Override
        public int addZCord(){
            return 1;
        }
    },
    SouthEast{
        @Override
        public int addXCord(){
            return 0;
        }

        @Override
        public int addYCord(){
            return 1;
        }

        @Override
        public int addZCord(){
            return -1;
        }
    },
    SouthWest{
        @Override
        public int addXCord(){
            return -1;
        }

        @Override
        public int addYCord(){
            return 1;
        }

        @Override
        public int addZCord(){
            return 0;
        }
    };

    public abstract int addXCord();
    public abstract int addYCord();
    public abstract int addZCord();
}
