#ifndef MENU_H
#define MENU_H

class Menu
{
public:
    virtual Menu* step() = 0;
    virtual ~Menu()
    {
    }
};

// Singleton menus. Used to return commands using step()

class PopMenu : public Menu
{
private:
    static Menu* retVal;
    PopMenu() {}
public:
    Menu* step() { return nullptr; }
    ~PopMenu() {}

    static Menu* get();
};

class ClearStack : public Menu
{
private:
    static Menu* retVal;
    ClearStack() {}
public:
    Menu* step() { return nullptr; }
    ~ClearStack() {}

    static Menu* get();
};

class SwitchStack : public Menu
{
private:
    static Menu* retVal;
    SwitchStack() {}
public:
    Menu* step() { return nullptr; }
    ~SwitchStack() {}

    static Menu* get();
};

#endif
