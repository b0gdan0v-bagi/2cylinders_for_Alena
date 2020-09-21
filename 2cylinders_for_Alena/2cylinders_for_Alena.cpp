#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <math.h>
#include <cmath>
#include <ostream>
using namespace sf;

Vector2i getCoord(RenderWindow &window, Vector2f resolution)
{
    Vector2i MouseCoords;
    MouseCoords.x = Mouse::getPosition(window).x;
    MouseCoords.y = Mouse::getPosition(window).y;
    MouseCoords.x -= 40;
    MouseCoords.y -= resolution.y / 2;
    MouseCoords.y *= -1;
    return MouseCoords;
}

void findEqDots(float fi, std::vector<CircleShape> &dots)
{
    float r1, r2, fiRes;
    float cylRadius = 20.f;
    double fieldConst = 5;
    CircleShape shape;
    shape.setFillColor(Color::Green);
    shape.setRadius(1.f);
    for (int i = 1; i<1280; ++i)
        for (int j = 1; j < 720; ++j)
        {
            r1 = sqrt(pow((i-40), 2) + pow(j-360, 2));
            r2 = sqrt(pow((i - 1250), 2) + pow(j - 360, 2));
            fiRes = -fieldConst * log(r1 / cylRadius) + fieldConst * log(r2 / cylRadius);
            if (fi > 0)
            {
                if ((fiRes < fi * 1.010) && (fiRes > fi * 0.990))
                {
                    shape.setPosition(i, j);
                    dots.push_back(shape);
                }
            }
            else
            {
                if ((fiRes > fi * 1.010) && (fiRes < fi * 0.990))
                {
                    shape.setPosition(i, j);
                    dots.push_back(shape);
                }
            }
        }
}

int main()
{
    Font font;
    if (!font.loadFromFile("TimesNewRoman.ttf"))
        return EXIT_FAILURE;

    
    double const PI = 3.14159265358;
    int d = 1210;
    Vector2f resolution;
    resolution.x = 1280;
    resolution.y = 720;
    RenderWindow window;
    window.create(VideoMode(resolution.x, resolution.y), "2 cyl");
    Texture hor, ver;
    if (!hor.loadFromFile("hor.png"))
    {
        // error...
    }
    if (!ver.loadFromFile("ver.png"))
    {
        // error...
    }

    Sprite horSprite, verSprite;
    horSprite.setTexture(hor);
    verSprite.setTexture(ver);
    sf::Vector2u verSize, horSize;  
    verSize = ver.getSize();
    horSize = hor.getSize();
    sf::Vector2u WindowSize = window.getSize();
    float verScaleX = (float) WindowSize.x / verSize.x;
    float verScaleY = (float) WindowSize.y / verSize.y;

    float horScaleX = (float) WindowSize.x / horSize.x;
    float horScaleY = (float) WindowSize.y / horSize.y;
    std::cout << (float) window.getSize().x / hor.getSize().x << "\n";
    std::cout << WindowSize.y << "\n";
    std::cout << horSize.y << "\n";
    verSprite.setScale(1, verScaleY);
    horSprite.setScale(horScaleX, 1);
    horSprite.setPosition(0, resolution.y - horSprite.getGlobalBounds().height);
    verSprite.getInverseTransform();
    //verSprite.rotate(180);
    //verSprite.setPosition(verSprite.getGlobalBounds().width, 0);

    float cylRadius = 20.f;
    CircleShape cyl1(cylRadius);
    CircleShape cyl2(cylRadius);
    cyl1.setPosition(40 - cylRadius, resolution.y / 2 - cylRadius);
    cyl2.setPosition(resolution.x - 50, resolution.y / 2 - cylRadius);
    cyl1.setFillColor(Color::Black);
    cyl2.setFillColor(Color::Black);

    RectangleShape lineX,lineY;
    lineX.setSize(Vector2f(resolution.x, 10));
    lineY.setSize(Vector2f(10, resolution.y));
    lineX.setPosition(0, resolution.y / 2 - 5);
    lineY.setPosition(40 - 5, 0);
    lineX.setFillColor(Color::Blue);
    lineY.setFillColor(Color::Blue);
    Text textX("X", font, 30), textY("Y", font, 30);
    Text mouseCoord("", font, 30);
    mouseCoord.setFillColor(Color::Black);
    mouseCoord.setPosition(80, 5);
    textX.setPosition(resolution.x - 30, resolution.y / 2+30);
    textY.setPosition(5, 1);
    textX.setFillColor(Color::Red);
    textY.setFillColor(Color::Red);
    std::vector<CircleShape> dots;
    std::vector<RectangleShape> lines;
    std::vector<std::vector<RectangleShape>> showLines;

    Vector2f cyl1Center(cyl1.getPosition().x + cylRadius, cyl1.getPosition().y + cylRadius);
    Vector2f cyl2Center(cyl2.getPosition().x + cylRadius, cyl2.getPosition().y + cylRadius);
    std::cout << cyl1Center.x << " " << cyl1Center.y << "\n" << cyl2Center.x << " " << cyl2Center.y << "\n";
    //40 , 360 - to start
    //1250, 360
    float r1, r2;
    double fieldConst = 5;
    double e1, e2, eOut, eOutX,eOutY;
    double fi, fi1, fi2;
    RectangleShape line(Vector2f(resolution.y, 10));
    line.rotate(90);
    line.setFillColor(Color::Blue);
    line.setPosition(45, 0);
    RectangleShape line2(Vector2f(resolution.x, 10));
    //line.rotate(90);
    line2.setFillColor(Color::Blue);
    line2.setPosition(0, resolution.y / 2 - 5);

    sf::Vertex lineToR1[] =
    {
        sf::Vertex(sf::Vector2f(10, 10)),
        sf::Vertex(sf::Vector2f(40, 360))
    };
    sf::Vertex lineToR2[] =
    {
        sf::Vertex(sf::Vector2f(10, 10)),
        sf::Vertex(sf::Vector2f(1250, 360))
    };

    RectangleShape lineE1(Vector2f(100, 2));
    lineE1.setFillColor(Color::Red);
    RectangleShape lineE2(Vector2f(100, 2));
    lineE2.setFillColor(Color::Red);
    RectangleShape lineESUM(Vector2f(100, 3));
    lineESUM.setFillColor(Color::Blue);
//    line.setPosition(45, 0);

    float angle1, angle2, angle0, angleEout;

    lineToR1[0].color = Color::Black;
    lineToR1[1].color = Color::Black;
    lineToR2[0].color = Color::Black;
    lineToR2[1].color = Color::Black;

    while (window.isOpen())
    {
        // Process events
        Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == Event::Closed)
                window.close();
            
        }
        // Clear screen
        window.clear(Color::White);

        std::ostringstream mouseCoordX, mouseCoordY, outR1, outR2, outE, outFi1, outFi2, outFi, smX, smY;
        r1 = sqrt(pow(Vector2f(Mouse::getPosition(window)).x - 40, 2) + pow((Vector2f(Mouse::getPosition(window)).y - 360), 2));
        r2 = sqrt(pow((Vector2f(Mouse::getPosition(window)).x - 1250), 2) + pow((Vector2f(Mouse::getPosition(window)).y - 360), 2));

        angle1 = acos(((pow(r1, 2)) + pow(d, 2) - pow(r2, 2)) / (2 * r1 * d)) * 180 / PI;
        angle2 = acos(((pow(r2, 2)) + pow(d, 2) - pow(r1, 2)) / (2 * r2 * d)) * 180 / PI;
        angle0 = acos(((pow(r1, 2)) + pow(r2, 2) - pow(1210, 2)) / (2 * r1 * r2)) * 180 / PI;
        e1 = fieldConst / r1;
        e2 = fieldConst / r2;

        eOutX = e1 * cos(angle1 * PI / 180) + e2 * cos(angle2 * PI / 180);
        eOutY = e1 * sin(angle1 * PI / 180) - e2 * sin(angle2 * PI / 180);

        eOut = sqrt(pow(eOutX, 2) + pow(eOutY, 2));
        angleEout = acos(eOutX / eOut) * 180 / PI;
        if (asin(eOutY / eOut) * 180 / PI < 0) angleEout *= -1;

        fi1 = -fieldConst * log(r1 / cylRadius);
        fi2 = fieldConst * log(r2 / cylRadius);
        fi = fi1 + fi2;
       // std::cout << fi1;
        outR1 << r1;
        outR2 << r2;
        
        outFi1 << fi1;
        outFi2 << fi2;
        float r1SM = sqrt(pow((Vector2f(Mouse::getPosition(window)).x - 40)/50 + 3.2, 2) +
            pow(((360 - (Mouse::getPosition(window).y))/40 ), 2));
        float r2SM = sqrt(pow((Vector2f(Mouse::getPosition(window)).x - 1250)/50 + 3.2, 2) + 
            pow(((360 - (Mouse::getPosition(window)).y)/40 ), 2));
        outFi << (-log(r1SM/1.5)+log(r2SM/1.5)+2.24)*14/4;
        outE << (fi + 23) * 14 / (43*r1);
        smX << 3.2 + Vector2f(Mouse::getPosition(window)).x / 50;
        smY << (720 - Vector2f(Mouse::getPosition(window)).y ) / 40;
        mouseCoordX << getCoord(window, resolution).x;
        mouseCoordY << getCoord(window, resolution).y;
        mouseCoord.setString("Mouse X : " + mouseCoordX.str() + "\nMouse Y : " + mouseCoordY.str() 
            + "\nR1 : " + outR1.str() + "\nR2 : " + outR2.str() + "\nOut E : " + outE.str() +
            "\nOut fi1 : " + outFi1.str() +
            "\nOut fi2 : " + outFi2.str() + 
            "\nOut fi : " + outFi.str() + 
        "\nSm X : " + smX.str() + " Sm Y : " + smY.str());


        lineToR1[0].position = Vector2f(Mouse::getPosition(window));
        lineToR2[0].position = Vector2f(Mouse::getPosition(window));
        
        lineE1.setPosition(Vector2f(Mouse::getPosition(window)));
        lineE1.setSize(Vector2f(e1*10000, 2));
        if (lineE1.getPosition().y < 360) lineE1.setRotation(-angle1);
        else  lineE1.setRotation(angle1);

        lineE2.setPosition(Vector2f(Mouse::getPosition(window)));
        lineE2.setSize(Vector2f(e2 * 10000, 2));
        if (lineE2.getPosition().y < 360) lineE2.setRotation(angle2);
        else  lineE2.setRotation(-angle2);
        
        lineESUM.setFillColor(Color::Blue);
        lineESUM.setPosition(Vector2f(Mouse::getPosition(window)));
        lineESUM.setSize(Vector2f(eOut * 10000, 2));
        if (lineESUM.getPosition().y < 360) lineESUM.setRotation(-angleEout);
        else  lineESUM.setRotation(angleEout);
        window.draw(lineESUM);

        if (Mouse::isButtonPressed(Mouse::Left))
        {
            while (Mouse::isButtonPressed(Mouse::Left)) {/* here is stop until mouse is unpressed */ }
            findEqDots(fi, dots);
            

        }
        float res, help1, help2;
        float newMousePosX, newMousePosY;
        res = modf((3.2 + Vector2f(Mouse::getPosition(window)).x / 50), &help1);
        if (res < 0.5)
        {
            newMousePosX = (3.2 + Vector2f(Mouse::getPosition(window)).x / 50) - res;
        }
        else
        {
            newMousePosX = (3.2 + Vector2f(Mouse::getPosition(window)).x / 50) - res + 1.f;
        }
        res = modf((Vector2f(Mouse::getPosition(window)).y) / 40, &help2);
        if (res < 0.5)
        {
            newMousePosY = (Vector2f(Mouse::getPosition(window)).y / 40) - res;
        }
        else
        {
            newMousePosY = (Vector2f(Mouse::getPosition(window)).y / 40) - res + 1.f;
        }
        if (Keyboard::isKeyPressed(Keyboard::Space))
        {
            Mouse::setPosition(Vector2i((newMousePosX - 3.2) * 50, newMousePosY * 40), window);
        }
        
            lineESUM.setSize(Vector2f(100, 1));
            lineESUM.setFillColor(Color::Green);
            Vector2f newPosOfLine = Vector2f(Mouse::getPosition(window));
            int count = 0;
            while ((newPosOfLine.x < cyl2.getGlobalBounds().left+cylRadius))
            {
                if (lineESUM.getPosition().y < 360)
                {
                    newPosOfLine.x += eOut * 50 * cos(-angleEout * PI / 180);
                    newPosOfLine.y += eOut * 50 * sin(-angleEout * PI / 180);
                }
                else
                {
                    newPosOfLine.x += eOut * 50 * cos(angleEout * PI / 180);
                    newPosOfLine.y += eOut * 50 * sin(angleEout * PI / 180);
                }
                r1 = sqrt(pow(newPosOfLine.x - 40, 2) + pow((newPosOfLine.y - 360), 2));
                r2 = sqrt(pow((newPosOfLine.x - 1250), 2) + pow((newPosOfLine.y - 360), 2));
                angle1 = acos(((pow(r1, 2)) + pow(d, 2) - pow(r2, 2)) / (2 * r1 * d)) * 180 / PI;
                angle2 = acos(((pow(r2, 2)) + pow(d, 2) - pow(r1, 2)) / (2 * r2 * d)) * 180 / PI;
                e1 = fieldConst / r1;
                e2 = fieldConst / r2;
                eOutX = e1 * cos(angle1 * PI / 180) + e2 * cos(angle2 * PI / 180);
                eOutY = e1 * sin(angle1 * PI / 180) - e2 * sin(angle2 * PI / 180);
                eOut = sqrt(pow(eOutX, 2) + pow(eOutY, 2));
                angleEout = acos(eOutX / eOut) * 180 / PI;
                if (asin(eOutY / eOut) * 180 / PI < 0) angleEout *= -1;
                lineESUM.setFillColor(Color::Green);
                lineESUM.setPosition(Vector2f(newPosOfLine));
                lineESUM.setSize(Vector2f(5, 1));
                if (lineESUM.getPosition().y < 360) lineESUM.setRotation(-angleEout);
                else  lineESUM.setRotation(angleEout);
                lines.push_back(lineESUM);
                count++;
                if (newPosOfLine.x > resolution.x) break;
                if (newPosOfLine.y == resolution.y/2) break;
                if (count > 10000) break;
            }

           /* while ((newPosOfLine.x > cyl1.getGlobalBounds().left + cylRadius))
            {
                if (lineESUM.getPosition().y < 360)
                {
                    newPosOfLine.x -= eOut * 50 * cos(-angleEout * PI / 180);
                    newPosOfLine.y -= eOut * 50 * sin(-angleEout * PI / 180);
                }
                else
                {
                    newPosOfLine.x -= eOut * 50 * cos(angleEout * PI / 180);
                    newPosOfLine.y -= eOut * 50 * sin(angleEout * PI / 180);
                }
                r1 = sqrt(pow(newPosOfLine.x - 40, 2) + pow((newPosOfLine.y - 360), 2));
                r2 = sqrt(pow((newPosOfLine.x - 1250), 2) + pow((newPosOfLine.y - 360), 2));
                angle1 = acos(((pow(r1, 2)) + pow(d, 2) - pow(r2, 2)) / (2 * r1 * d)) * 180 / PI;
                angle2 = acos(((pow(r2, 2)) + pow(d, 2) - pow(r1, 2)) / (2 * r2 * d)) * 180 / PI;
                e1 = fieldConst / r1;
                e2 = fieldConst / r2;
                eOutX = e1 * cos(angle1 * PI / 180) + e2 * cos(angle2 * PI / 180);
                eOutY = e1 * sin(angle1 * PI / 180) - e2 * sin(angle2 * PI / 180);
                eOut = sqrt(pow(eOutX, 2) + pow(eOutY, 2));
                angleEout = acos(eOutX / eOut) * 180 / PI;
                if (asin(eOutY / eOut) * 180 / PI < 0) angleEout *= -1;
                lineESUM.setFillColor(Color::Green);
                lineESUM.setPosition(Vector2f(newPosOfLine));
                lineESUM.setSize(Vector2f(5, 1));
                if (lineESUM.getPosition().y < 360) lineESUM.setRotation(-angleEout);
                else  lineESUM.setRotation(angleEout);
                lines.push_back(lineESUM);
                count++;
                if (newPosOfLine.x < 0) break;
                if (newPosOfLine.y == resolution.y / 2) break;
                if (count > 10000) break;
            }*/

            if (Mouse::isButtonPressed(Mouse::Right))
            {
                while (Mouse::isButtonPressed(Mouse::Right)) {/* here is stop until mouse is unpressed */ }
                for (int i = 0; i < lines.size(); ++i)
                {
                    lines.at(i).setFillColor(Color::Black);
                }
                showLines.push_back(lines);
            }
       

        
        //window.draw(horSprite);
        //window.draw(verSprite);
        window.draw(lineX);
        window.draw(lineY);
        window.draw(line);
        window.draw(line2);

        window.draw(cyl1);
        window.draw(cyl2);
        window.draw(textX);
        window.draw(textY);
        window.draw(mouseCoord);
        for (int i = 0; i < dots.size(); ++i)
        {
            window.draw(dots.at(i));
        }

        for (int i = 0; i < lines.size(); ++i)
        {
            window.draw(lines.at(i));
        }
        for (int i = 0; i < showLines.size(); ++i)
            for (int j = 0; j < showLines.at(i).size(); ++j)
        {
            window.draw(showLines[i][j]);
        }
        lines.clear();
        window.draw(lineToR1, 2, sf::Lines);
        window.draw(lineToR2, 2, sf::Lines);
        window.draw(lineE1);
        window.draw(lineE2);
        
        window.display();
    }
    return EXIT_SUCCESS;
}