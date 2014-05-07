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
    MandelbrotWidget * topPanel;
    MandelbrotWidget * bottomPanel;
    QVBoxLayout * layout;

    TwoPaneledImageWidget(){
      window = new QWidget();
      window->setWindowTitle(
            QApplication::translate("toplevel", "Top-level widget"));
      window->resize(320, 240);
      topPanel = new MandelbrotWidget();
      bottomPanel = new MandelbrotWidget();

      QObject::connect( topPanel, SIGNAL(coorschanged(int,int)),
                        bottomPanel, SLOT(coors(int,int)));
      QObject::connect( bottomPanel, SIGNAL(coorschanged(int,int)),
                        topPanel, SLOT(coors(int,int)) );

      layout = new QVBoxLayout();
      layout->addWidget(topPanel);
      layout->addWidget(bottomPanel);
      window->setLayout(layout);
    }

    ~TwoPaneledImageWidget(){
      delete layout;
      delete topPanel;
      delete bottomPanel;
      delete window;
    }

    //set bottom map
    void setBottomMap(ralab::MapVis * mp){
      bottomPanel->setMap(mp);
    }

    // set top map
    void setTopMap(ralab::MapVis * mp){
      topPanel->setMap(mp);
    }

    // show
    void show(){
      window->show();
    }
  };
}

#endif
