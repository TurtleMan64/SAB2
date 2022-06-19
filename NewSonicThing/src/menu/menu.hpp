#ifndef MENU_H
#define MENU_H

class Menu
{
public:
    virtual Menu* step() = 0;
    virtual ~Menu() {};
    virtual void draw();
    virtual void setVisible(bool);
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
    void draw() {}
    void setVisible(bool) {}

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
    void draw() {}
    void setVisible(bool) {}

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
    void draw() {}
    void setVisible(bool) {}

    static Menu* get();
};

#endif
