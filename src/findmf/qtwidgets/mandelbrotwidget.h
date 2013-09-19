// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT



#ifndef MANDELBROTWIDGET_H
#define MANDELBROTWIDGET_H

#include <QPixmap>
#include <QWidget>
#include <boost/shared_ptr.hpp>

#include "renderthread.h"

//! [0]
class MandelbrotWidget : public QWidget
{
  Q_OBJECT
public:
  MandelbrotWidget(QWidget *parent = 0);
  ///pass the map to the renderer
  void setMap(ralab::MapVis * map);
  ~MandelbrotWidget(){};
protected:
  void paintEvent(QPaintEvent *event);
  void resizeEvent(QResizeEvent *event);
  void keyPressEvent(QKeyEvent *event);
  void wheelEvent(QWheelEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);

signals:
  void coorschanged(int xcoor, int ycoor);

 public slots:
  void coors(int xcoor, int ycoor);
  void updatePixmap(const QImage &image, double scaleFactor);
  void zoom(double zoomFactor);

private:
  void scroll(int deltaX, int deltaY);
  RenderThread thread;
  QPixmap pixmap;
  QPoint pixmapOffset;
  QPoint lastDragPos;
  double centerX_;
  double centerY_;
  double pixmapScale;
  double curScale_;
};
#endif
