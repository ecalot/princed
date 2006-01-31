#ifndef _GUARD_H_
#define _GUARD_H_

typedef enum {left=0,right=-1} Direction;

class Guard {
public:
       Guard(Direction direction,int  skill,int color); //TODO: use an abstract class for pop1 and pop2
       int getSkill();
       int getColour();
       Direction getDirection();

private:
        Direction direction;
        int skill;      
        int color;
      
      
      
      
};

#endif
