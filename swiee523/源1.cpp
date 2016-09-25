void MouseActions::MMoveMoving2(Schematic *Doc, QMouseEvent *Event)
{
	setPainter(Doc);

	MAx2 = DOC_X_POS(Event->pos().x());
	MAy2 = DOC_Y_POS(Event->pos().y());

	Element *pe;
	if(drawn) // erase old scheme
		for(pe = movingElements.first(); pe != 0; pe = movingElements.next())
			pe->paintScheme(Doc);
	//      if(pe->Type == isWire)  if(((Wire*)pe)->Label)
	//        if(!((Wire*)pe)->Label->isSelected)
	//          ((Wire*)pe)->Label->paintScheme(&painter);

	Element *wmt;
	for(wmt = movingElements.first(); wmt != 0; wmt = movingElements.next()) 
	{
		//ÅÐ¶ÏÊÇ·ñÊÇÏß
		if(wmt->Type == isWire)
		{
			Doc->PostPaintEvent(_DotLine,wmt->x1,wmt->y1,wmt->x2,wmt->y2,0,0,true);
		}
	}

	drawn = true;
	if((Event->state() & Qt::ControlModifier) == 0)
		Doc->setOnGrid(MAx2, MAy2);  // use grid only if CTRL key not pressed
	MAx1 = MAx2 - MAx1;
	MAy1 = MAy2 - MAy1;
	MAx3 += MAx1;  MAy3 += MAy1;   // keep track of the complete movement

	moveElements(&movingElements, MAx1, MAy1);  // moves elements by MAx1/MAy1

	// paint afterwards to avoid conflict between wire and label painting
	for(pe = movingElements.first(); pe != 0; pe = movingElements.next())
		pe->paintScheme(Doc);
	//    if(pe->Type == isWire)  if(((Wire*)pe)->Label)
	//      if(!((Wire*)pe)->Label->isSelected)
	//        ((Wire*)pe)->Label->paintScheme(&painter);

	MAx1 = MAx2;
	MAy1 = MAy2;
}

void Schematic::drawContents(QPainter *p, int, int, int, int)
{
  ViewPainter Painter;
  Painter.init(p, Scale, -ViewX1, -ViewY1, contentsX(), contentsY());

  paintGrid(&Painter, contentsX(), contentsY(),
            visibleWidth(), visibleHeight());

  if(!symbolMode)
    paintFrame(&Painter);

  for(Component *pc = Components->first(); pc != 0; pc = Components->next())
    pc->paint(&Painter);

  for(Wire *pw = Wires->first(); pw != 0; pw = Wires->next()) {
    pw->paint(&Painter);
    if(pw->Label)
      pw->Label->paint(&Painter);  // separate because of paintSelected
  }

  Node *pn;
  for(pn = Nodes->first(); pn != 0; pn = Nodes->next()) {
    pn->paint(&Painter);
    if(pn->Label)
      pn->Label->paint(&Painter);  // separate because of paintSelected
  }

  // FIXME disable here, issue with select box goes away
  // also, instead of red, line turns blue
//  for(Diagram *pd = Diagrams->first(); pd != 0; pd = Diagrams->next()) // original, comment out by xuliang
//    pd->paint(&Painter);
	for(Diagram *pd = Diagrams->first(); pd != 0; pd = Diagrams->next())
	{
		if(pd->Name == "Running") ((AnimateDiagram *)pd)->paintAD(&Painter);
		else pd->paint(&Painter);
	}

  for(Painting *pp = Paintings->first(); pp != 0; pp = Paintings->next())
    pp->paint(&Painter);

  if(showBias > 0) {  // show DC bias points in schematic ?
    int x, y, z;
    for(pn = Nodes->first(); pn != 0; pn = Nodes->next()) {
      if(pn->Name.isEmpty()) continue;
      x = pn->cx;
      y = pn->cy + 4;
      z = pn->x1;
      if(z & 1) x -= Painter.Painter->fontMetrics().width(pn->Name);
      if(!(z & 2)) {
        y -= (Painter.LineSpacing>>1) + 4;
        if(z & 1) x -= 4;
        else x += 4;
      }
      if(z & 0x10)
        Painter.Painter->setPen(Qt::darkGreen);  // green for currents
      else
        Painter.Painter->setPen(Qt::blue);   // blue for voltages
      Painter.drawText(pn->Name, x, y);
    }
  }

  /*
   * The following events used to be drawn from mouseactions.cpp, but since Qt4
   * Paint actions can only be called from within the paint event, so they
   * are put into a QList (PostedPaintEvents) and processed here
   */
  for(int i=0;i<PostedPaintEvents.size();i++)
  {
    PostedPaintEvent p = PostedPaintEvents[i];
    QPainter painter2(viewport());

    switch(p.pe)
    {
      case _NotRop:
        if(p.PaintOnViewport)
          painter2.setCompositionMode(QPainter::RasterOp_SourceAndNotDestination);
        else
          Painter.Painter->setCompositionMode(QPainter::RasterOp_SourceAndNotDestination);
        break;
      case _Rect:
        if(p.PaintOnViewport)
          painter2.drawRect(p.x1, p.y1, p.x2, p.y2);
        else
          Painter.drawRect(p.x1, p.y1, p.x2, p.y2);
        break;
      case _Line:
        if(p.PaintOnViewport)
          painter2.drawLine(p.x1, p.y1, p.x2, p.y2);
        else
          Painter.drawLine(p.x1, p.y1, p.x2, p.y2);
        break;
      case _Ellipse:
        if(p.PaintOnViewport)
          painter2.drawEllipse(p.x1, p.y1, p.x2, p.y2);
        else
          Painter.drawEllipse(p.x1, p.y1, p.x2, p.y2);
        break;
      case _Arc:
        if(p.PaintOnViewport)
          painter2.drawArc(p.x1, p.y1, p.x2, p.y2, p.a, p.b);
        else
          Painter.drawArc(p.x1, p.y1, p.x2, p.y2, p.a, p.b);
        break;
      case _DotLine:
        Painter.Painter->setPen(QPen(Qt::gray,1,Qt::DotLine));
		Painter.drawLine(p.x1, p.y1, p.x2, p.y2);
        break;
      case _Translate:

        painter2.translate(p.x1, p.y1);
        break;
      case _Scale:
        painter2.scale(p.x1,p.y1);
        break;
    }

  }
  PostedPaintEvents.clear();

}
