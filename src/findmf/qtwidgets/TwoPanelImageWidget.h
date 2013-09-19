// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT



#ifndef _TWOPANELIMAGEWIDGET
#define _TWOPANELIMAGEWIDGET
#include <QtGui>
#include "mandelbrotwidget.h"


namespace ralab{
  struct TwoPaneledImageWidget : QObject{
    QWidget * window;
    MandelbrotWidget * widget;
    MandelbrotWidget * widget2;
    QVBoxLayout * layout;

    TwoPaneledImageWidget(){
      window = new QWidget();
      window->setWindowTitle(
            QApplication::translate("toplevel", "Top-level widget"));
      window->resize(320, 240);
      widget = new MandelbrotWidget();
      widget2 = new MandelbrotWidget();

      QObject::connect( widget, SIGNAL(coorschanged(int,int)),
                        widget2, SLOT(coors(int,int)));
      QObject::connect( widget2, SIGNAL(coorschanged(int,int)),
                        widget, SLOT(coors(int,int)) );

      layout = new QVBoxLayout();
      layout->addWidget(widget);
      layout->addWidget(widget2);
      window->setLayout(layout);
    }

    ~TwoPaneledImageWidget(){
      delete layout;
      delete widget;
      delete widget2;
      delete window;
    }

    //set bottom map
    void setBottomMap(ralab::MapVis * mp){
      widget2->setMap(mp);
    }

    // set top map
    void setTopMap(ralab::MapVis * mp){
      widget->setMap(mp);
    }

    // show
    void show(){
      window->show();
    }
  };
}

#endif
