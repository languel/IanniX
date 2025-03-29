/*
    This file is part of IanniX, a graphical real-time open-source sequencer for digital art
    Copyright (C) 2010-2015 — IanniX Association

    Project Manager: Thierry Coduys (http://www.le-hub.org)
    Development:     Guillaume Jacquemin (https://www.buzzinglight.com)

    This file was written by Guillaume Jacquemin.

    IanniX is a free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "nxcursor.h"

NxCursor::NxCursor(ApplicationCurrent *parent, QTreeWidgetItem *ccParentItem) :
    NxObject(parent, ccParentItem) {
    setText(0, tr("CURSOR"));
    glListCursor = glGenLists(1);
    curve = 0;
    nextTimeOld = 0;
    timeOld = 0;
    fire = 2;
    time = 0;
    timeLocal = 0;
    timeLocalOld = 0;
    timeLocalAbsolute = 0;
    previousCursorReliable = previousPreviousCursorReliable = false;
    cursorAngleCacheSinZ = cursorAngleCacheCosZ = cursorAngleCacheSinY = cursorAngleCacheCosY = 0;
    cursorPoly       = NxPolygon(4);
    cursorPoly[0]    = NxPoint();
    cursorPoly[1]    = NxPoint();
    cursorPoly[2]    = NxPoint();
    cursorPoly[3]    = NxPoint();
    cursorPolyOld    = NxPolygon(4);
    cursorPolyOld[0] = NxPoint();
    cursorPolyOld[1] = NxPoint();
    cursorPolyOld[2] = NxPoint();
    cursorPolyOld[3] = NxPoint();
    cursorPolyOldOld    = NxPolygon(4);
    cursorPolyOldOld[0] = NxPoint();
    cursorPolyOldOld[1] = NxPoint();
    cursorPolyOldOld[2] = NxPoint();
    cursorPolyOldOld[3] = NxPoint();

    initializeCustom();
}
void NxCursor::initializeCustom() {
    setSize(1);
    setColorActive  ("_cursor_active");
    setColorInactive("_cursor_inactive");
    setOffset("0 0 end");
    setStart("0 0 1 0");
    setTextureActive  ("cursor_active");
    setTextureInactive("cursor_inactive");
    setBoundsSource("-10 10 -10 10 -10 10");
    setBoundsTarget("0 1 0 1 0 1");
    setBoundsSourceMode(1);
    setTimeFactorStr("1");
    setTimeFactorF(1);
    setWidth(1);
    setDepth(0.5);
    setTimeLocal(0);
    setMessagePatterns("20," + Application::defaultMessageCursor.val());
}
NxCursor::~NxCursor() {
    glDeleteLists(glListCursor, 1);
}
void NxCursor::setTime(qreal delta) {
    previousPreviousCursorReliable = previousCursorReliable;

    if((curve) && (start.count())) {
        //toto += delta;



        //totoPt = curve->getPointAt(toto, true);

        //qDebug("%f \t %f \t %f %f %f", delta, toto, totoPt.x(), totoPt.y(), totoPt.z());





        timeLocalOld = timeLocal;

        qint16 indexOfZero = start.indexOf(0);
        qreal loopFactor   = start.at(nbLoop % start.count());
        if((indexOfZero > 0) && (nbLoop > indexOfZero))
            loopFactor = 0;

        factors = timeFactor * timeFactorF * loopFactor;
        timeLocalAbsolute += delta * timeFactor * timeFactorF * qAbs(loopFactor);
        if(time >= 0)
            timeLocal     += delta * factors;
        else
            timeLocal = 0;

        qreal timeInitialOffsetReal = timeInitialOffset * qAbs(factors);
        qreal timeStartOffsetReal   = timeStartOffset  ;// * qAbs(factors);
        qreal timeEndOffsetReal     = timeEndOffset    ;// * qAbs(factors);
        qreal timeLocalAbsoluteCopy = timeLocalAbsolute + timeInitialOffsetReal;
        qreal fakeCurveLength = curve->getPathLength() - timeStartOffsetReal;

        if(timeEndOffset > 0)
            fakeCurveLength = timeEndOffsetReal - timeStartOffsetReal;
        nbLoop = 0;
        if(fakeCurveLength > 0) {
            if(timeLocalAbsoluteCopy > 0) {
                while(timeLocalAbsoluteCopy > fakeCurveLength) {
                    if((indexOfZero > 0) && (nbLoop >= (indexOfZero-1)))
                        break;
                    else {
                        nbLoop++;
                        timeLocalAbsoluteCopy -= fakeCurveLength;
                    }
                }
            }
            else {
                while(timeLocalAbsoluteCopy < timeInitialOffsetReal) {
                    if((indexOfZero > 0) && (nbLoop >= (indexOfZero-1)))
                        break;
                    else {
                        nbLoop++;
                        timeLocalAbsoluteCopy += fakeCurveLength;
                    }
                }
            }
        }

        //Preparation of time difference
        if(!previousCursorReliable) timeOld = nextTimeOld;
        else                        timeOld = time;
        nextTimeOld = timeOld;
        previousCursorReliable = true;

        //Time calculation
        if(loopFactor >= 0)
            time = timeLocalAbsoluteCopy / fakeCurveLength;
        else
            time = (fakeCurveLength - timeLocalAbsoluteCopy) / fakeCurveLength;

        if((time < 0) || (time > 1))
            previousCursorReliable = false;

        //Loop
        if(nbLoop != nbLoopOld) {
            previousCursorReliable = false;
            nextTimeOld = qRound(time)    / curve->getPathLength() * fakeCurveLength + timeStartOffsetReal / curve->getPathLength();
            time        = qRound(timeOld) / curve->getPathLength() * fakeCurveLength + timeStartOffsetReal / curve->getPathLength();
        }
        else {
            time = time / curve->getPathLength() * fakeCurveLength + timeStartOffsetReal / curve->getPathLength();
        }

        //Finaly
        nbLoopOld = nbLoop;
        calculate();

        //Activity
        if(qAbs(timeLocal - timeLocalOld) < 0.00001) {
            if(!hasActivityOld)
                hasActivity = false;
            hasActivityOld = false;
        }
        else {
            hasActivity = true;
            hasActivityOld = true;
        }
    }
}


void NxCursor::calculate() {
    //Cursor line
    if((curve) && (curve->getPathLength() > 0)) {
        qreal timeReal = easing.getValue(time), timeOldReal = easing.getValue(timeOld);
        cursorPos = curve->getPointAt(timeReal) + curve->getPos();

        if(timeReal == 0)
            cursorAngle = -curve->getAngleAt(timeReal + 0.001);
        else if(timeReal == 1)
            cursorAngle = -curve->getAngleAt(timeReal - 0.001);
        else
            cursorAngle = -curve->getAngleAt(timeReal);

        cursorPosOld = curve->getPointAt(timeOldReal) + curve->getPos();

        if(timeOldReal == 0)
            cursorAngleOld = -curve->getAngleAt(timeOldReal + 0.001);
        else if(timeOldReal == 1)
            cursorAngleOld = -curve->getAngleAt(timeOldReal - 0.001);
        else
            cursorAngleOld = -curve->getAngleAt(timeOldReal);

        //Infos en +
        //NxPoint cursorPosDelta = cursorPosOld - cursorPos;
        //cursorAngleRoll = 0;//-qSin(cursorPosDelta.x() * M_PI) * 180 * 2;
        //cursorAnglePitch = 0;//qSin(cursorPosDelta.y() * M_PI) * 180 * 5;
    }
    else {
        cursorPos = pos;

        NxPoint cursorPosDelta = cursorPosOld - cursorPos;
        previousCursorReliable = true;

        qreal angleZ = -qAtan2(cursorPosDelta.x(), cursorPosDelta.y()) * 180.0F / M_PI + 90;
        qreal angleY =  qAtan2(qSqrt(cursorPosDelta.x()*cursorPosDelta.x() + cursorPosDelta.y()*cursorPosDelta.y()), cursorPosDelta.z()) * 180.0F / M_PI + 90;
        cursorAngle = NxPoint(0, angleY, angleZ);
    }

    if(cursorAngle != cursorAngle)
        cursorAngle = NxPoint(0, 0, 0);
    cursorRelativePos = getCursorValue(cursorPos);

    //Aed
    qreal cursorDistance = qSqrt(cursorPos.x()*cursorPos.x() + cursorPos.y()*cursorPos.y() + cursorPos.z()*cursorPos.z());
    cursorAed = NxPoint(0, 0, cursorDistance);
    if((cursorPos.x() != 0) && (cursorPos.y() != 0))    cursorAed.setX(qAtan2(cursorPos.y(), cursorPos.x()) * 180. / M_PI - 90);
    if((cursorPos.z() != 0) && (cursorDistance != 0))   cursorAed.setY(qAtan2(cursorPos.z(), cursorDistance) * 180. / M_PI);
    while(cursorAed.x() < 0)    cursorAed.setX(cursorAed.x() + 360);
    qreal cursorRelativeDistance = qSqrt(cursorRelativePos.x()*cursorRelativePos.x() + cursorRelativePos.y()*cursorRelativePos.y() + cursorRelativePos.z()*cursorRelativePos.z());
    cursorRelativeAed = NxPoint(0, 0, cursorRelativeDistance);
    if((cursorRelativePos.x() != 0) && (cursorRelativePos.y() != 0))    cursorRelativeAed.setX(qAtan2(cursorRelativePos.y(), cursorRelativePos.x()) * 180. / M_PI - 90);
    if((cursorRelativePos.z() != 0) && (cursorRelativeDistance != 0))   cursorRelativeAed.setY(qAtan2(cursorRelativePos.z(), cursorRelativeDistance) * 180. / M_PI);
    while(cursorRelativeAed.x() < 0)    cursorRelativeAed.setX(cursorRelativeAed.x() + 360);


    //Cursor
    cursorPolyOldOld = cursorPolyOld;
    cursorPolyOld = cursorPoly;
    cursorPoly.replace(0, NxPoint(0, -width/2, -depth/2));
    cursorPoly.replace(1, NxPoint(0, -width/2,  depth/2));
    cursorPoly.replace(2, NxPoint(0,  width/2,  depth/2));
    cursorPoly.replace(3, NxPoint(0,  width/2, -depth/2));
    //Rotations + translations
    qreal angle, angleSin, angleCos;
    angle = cursorAngle.y() * M_PI / 180., angleSin = qSin(angle), angleCos = qCos(angle);
    cursorAngleCacheSinY = -angleSin;
    cursorAngleCacheCosY =  angleCos;
    for(quint16 i = 0 ; i < cursorPoly.count() ; i++)
        cursorPoly.replace(i, NxPoint(cursorPoly.at(i).z() * angleSin + cursorPoly.at(i).x() * angleCos,
                                      cursorPoly.at(i).y(),
                                      cursorPoly.at(i).z() * angleCos - cursorPoly.at(i).x() * angleSin));
    angle = cursorAngle.z() * M_PI / 180., angleSin = qSin(angle), angleCos = qCos(angle);
    cursorAngleCacheSinZ = -angleSin;
    cursorAngleCacheCosZ =  angleCos;
    for(quint16 i = 0 ; i < cursorPoly.count() ; i++)
        cursorPoly.replace(i, NxPoint(cursorPos.x() + cursorPoly.at(i).x() * angleCos - cursorPoly.at(i).y() * angleSin,
                                      cursorPos.y() + cursorPoly.at(i).x() * angleSin + cursorPoly.at(i).y() * angleCos,
                                      cursorPos.z() + cursorPoly.at(i).z()));

    if((!previousCursorReliable) || (!previousPreviousCursorReliable))
        cursorPolyOld = cursorPolyOldOld = cursorPoly;


    calcBoundingRect();

    if((cursorPos.sx() != cursorPosOld.sx()) || (cursorPos.sy() != cursorPosOld.sy()) || (cursorPos.sz() != cursorPosOld.sz()))
        glListRecreate = true;
    if((curve) && (curve->getPathLength() > 0)) {
    }
    else {
        cursorPosOld = cursorPos;
        cursorAngleOld = cursorAngle;
    }
}


void NxCursor::paint() {
    //Color
    if(active) {
        if(colorActive.isEmpty())                                                                                       color = colorActiveColor;
        else if((colorActive.startsWith("_")) && (Render::colors->contains(Application::colorsPrefix() + colorActive)))      color = Render::colors->value(Application::colorsPrefix() + colorActive);
        else if(Render::colors->contains(colorActive))                                                                  color = Render::colors->value(colorActive);
        else                                                                                                            color = Qt::gray;
    }
    else {
        if(colorInactive.isEmpty())                                                                                     color = colorInactiveColor;
        else if((colorInactive.startsWith("_")) && (Render::colors->contains(Application::colorsPrefix() + colorInactive)))  color = Render::colors->value(Application::colorsPrefix() + colorInactive);
        else if(Render::colors->contains(colorInactive))                                                                color = Render::colors->value(colorInactive);
        else                                                                                                            color = Qt::gray;
    }
    color.setRgb (qBound(qreal(0.), color.red()   * colorMultiplyColor.redF(),   qreal(255.)),
                  qBound(qreal(0.), color.green() * colorMultiplyColor.greenF(), qreal(255.)),
                  qBound(qreal(0.), color.blue()  * colorMultiplyColor.blueF(),  qreal(255.)),
                  qBound(qreal(0.), color.alpha() * colorMultiplyColor.alphaF(), qreal(255.)));

    if(color.alpha() > 0) {
        //Mouse hover
        if(selectedHover)   color = Render::colors->value(Application::colorsPrefix() + "_gui_object_hover");
        if(selected)        color = Render::colors->value(Application::colorsPrefix() + "_gui_object_selection");

        //Start
        if(!Render::paintThisGroup)
            color.setAlphaF(0.1);

        if(!Application::allowSelectionCursors)
            color.setAlphaF(color.alphaF()/3);

        glColor4f(color.redF(), color.greenF(), color.blueF(), color.alphaF());

        //Cursor chasse-neige
        if((0.0F <= time) && (time <= 1.0F) && (start.count()) && (start.at(nbLoop % start.count()) != 0)) {
            //Label
            if((Render::paintThisGroup) && (Application::paintLabel || selectedHover) && (!label.isEmpty()))
                Application::render->renderText(cursorPos.x() + 0.2, cursorPos.y() + 0.2, cursorPos.z(), QString::number(id) + " - " + label.toUpper(), Application::renderFont, true);
            else if(selectedHover)
                Application::render->renderText(cursorPos.x() + 0.2, cursorPos.y() + 0.2, cursorPos.z(), QString::number(id), Application::renderFont, true);
            if((selectedHover) && (!isDrag)) {
                qreal startY = -0.1;
                foreach(const QString & messageLabelItem, messageLabel) {
                    Application::render->renderText(cursorPos.x() + 0.2, cursorPos.y() + startY, cursorPos.z(), messageLabelItem.trimmed(), Application::renderFont, true);
                    startY -= 0.2 * Render::zoomLinear;
                }
            }

            //Draw
            bool textureOk = false;
            QString textureName = (active)?(textureActive):(textureInactive);
            if(Render::textures->contains(textureName)) {
                UiRenderTexture *texture = Render::textures->value(textureName);
                if((texture) && (texture->loaded) && (texture->mapping.width() != 0 ) && (texture->mapping.height() != 0)) {
                    textureOk = true;

                    glPushMatrix();
                    glTranslatef(cursorPos.x(), cursorPos.y(), cursorPos.z());
                    glRotatef(cursorAngle.z(), 0, 0, 1);
                    glRotatef(cursorAngle.y(), 0, 1, 0);
                    glRotatef(cursorAngle.x(), 1, 0, 0);

                    if(texture->isSyphon) {
                        glEnable(GL_TEXTURE_RECTANGLE_ARB);
                        glBindTexture(GL_TEXTURE_RECTANGLE_ARB, texture->texture);
                        glBegin(GL_QUADS);
                        qreal widthRatio = width * texture->originalSize.width() / texture->originalSize.height();
                        glTexCoord2d(0, 0); glVertex3f(widthRatio * texture->mapping.left(),  width/2 * texture->mapping.bottom(), 0);
                        glTexCoord2d(texture->originalSize.width(), 0); glVertex3f(0, width/2 * texture->mapping.bottom(), 0);
                        glTexCoord2d(texture->originalSize.width(), texture->originalSize.height()); glVertex3f(0, width/2 * texture->mapping.top(), 0);
                        glTexCoord2d(0, texture->originalSize.height()); glVertex3f(widthRatio * texture->mapping.left(),  width/2 * texture->mapping.top(), 0);
                        glEnd();
                        glDisable(GL_TEXTURE_RECTANGLE_ARB);
                    }
                    else {
                        glEnable(GL_TEXTURE_2D);
                        glBindTexture(GL_TEXTURE_2D, texture->texture);
                        qreal widthRatio = width * texture->originalSize.width() / texture->originalSize.height();
                        glBegin(GL_QUADS);
                        glTexCoord2d(0, 0); glVertex3f(widthRatio * texture->mapping.left(),  width/2 * texture->mapping.bottom(), 0);
                        glTexCoord2d(1, 0); glVertex3f(0, width/2 * texture->mapping.bottom(), 0);
                        glTexCoord2d(1, 1); glVertex3f(0, width/2 * texture->mapping.top(), 0);
                        glTexCoord2d(0, 1); glVertex3f(widthRatio * texture->mapping.left(),  width/2 * texture->mapping.top(), 0);
                        glEnd();
                        glDisable(GL_TEXTURE_2D);
                    }

                    glEnd();
                    glPopMatrix();
                }
            }
            if(!textureOk) {
                //Cursor
                glLineWidth(OpenGlDrawing::dpi * size);
                glEnable(GL_LINE_STIPPLE);
                glLineStipple(lineFactor, lineStipple);
                if(depth == 0) {
                    if(size > 0) {
                        glBegin(GL_LINE_STRIP);
                        glVertex3f(cursorPoly.at(1).x(), cursorPoly.at(1).y(), cursorPoly.at(1).z());
                        glVertex3f(cursorPoly.at(2).x(), cursorPoly.at(2).y(), cursorPoly.at(2).z());
                        glEnd();
                    }
                }
                else {
                    glColor4f(color.redF(), color.greenF(), color.blueF(), color.alphaF()/5);
                    glBegin(GL_QUADS);
                    glVertex3f(cursorPoly.at(0).x(), cursorPoly.at(0).y(), cursorPoly.at(0).z());
                    glVertex3f(cursorPoly.at(1).x(), cursorPoly.at(1).y(), cursorPoly.at(1).z());
                    glVertex3f(cursorPoly.at(2).x(), cursorPoly.at(2).y(), cursorPoly.at(2).z());
                    glVertex3f(cursorPoly.at(3).x(), cursorPoly.at(3).y(), cursorPoly.at(3).z());
                    glEnd();

                    if(size > 0) {
                        glColor4f(color.redF(), color.greenF(), color.blueF(), color.alphaF());
                        glBegin(GL_LINE_LOOP);
                        glVertex3f(cursorPoly.at(0).x(), cursorPoly.at(0).y(), cursorPoly.at(0).z());
                        glVertex3f(cursorPoly.at(1).x(), cursorPoly.at(1).y(), cursorPoly.at(1).z());
                        glVertex3f(cursorPoly.at(2).x(), cursorPoly.at(2).y(), cursorPoly.at(2).z());
                        glVertex3f(cursorPoly.at(3).x(), cursorPoly.at(3).y(), cursorPoly.at(3).z());
                        glEnd();
                    }
                }
                glDisable(GL_LINE_STIPPLE);
                glLineWidth(OpenGlDrawing::dpi);

                //Cursor reader
                glPushMatrix();
                glTranslatef(cursorPos.x(), cursorPos.y(), cursorPos.z());
                glRotatef(cursorAngle.z(), 0, 0, 1);
                glRotatef(cursorAngle.y(), 0, 1, 0);
                glRotatef(cursorAngle.x(), 1, 0, 0);
                qreal size2 = Render::objectSize / 2 * qMin(qreal(1.), width);
                glBegin(GL_TRIANGLE_FAN);
                glLineWidth(OpenGlDrawing::dpi * 2);
                if(hasActivity) {
                    if((time - timeOld) >= 0)  glVertex3f(size2, 0, 0);
                    else                       glVertex3f(-size2, 0, 0);
                }
                glVertex3f(0, -size2, 0);
                glVertex3f(0, size2, 0);
                glLineWidth(OpenGlDrawing::dpi);
                glEnd();
                glPopMatrix();

                //Special feature YEOSU
                if((true) && ((cursorPos.sx()) || (cursorPos.sy()) || (cursorPos.sz()))) {
                    glPushMatrix();
                    glTranslatef(cursorPos.x(), cursorPos.y(), cursorPos.z());
                    glColor4f(color.redF(), color.greenF(), color.blueF(), color.alphaF() / 8.F);
                    if(curve)
                        glLineWidth(OpenGlDrawing::dpi * curve->getSize());
                    else
                        glLineWidth(OpenGlDrawing::dpi);

                    if((glListRecreate) || (Render::forceLists)) {
                        glNewList(glListCursor, GL_COMPILE_AND_EXECUTE);
                        qreal lats = 40, longs = 40;
                        qreal rx = cursorPos.sx(), ry = cursorPos.sy(), rz = cursorPos.sz();
                        glBegin(GL_LINE_STRIP);
                        for(quint16 i = 0; i <= lats; i++) {
                            qreal lat0 = M_PI * (-0.5 + (qreal)(i - 1) / lats);
                            qreal lat1 = M_PI * (-0.5 + (qreal)(i    ) / lats);
                            qreal z0  = qSin(lat0) * rz, zr0 = qCos(lat0);
                            qreal z1  = qSin(lat1) * rz, zr1 = qCos(lat1);

                            for(quint16 j = 0; j <= longs; j++) {
                                qreal lng = 2 * M_PI * (qreal)(j - 1) / longs;
                                qreal x = qCos(lng) * rx;
                                qreal y = qSin(lng) * ry;
                                glNormal3f(x * zr0, y * zr0, z0);
                                glVertex3f(x * zr0, y * zr0, z0);
                                glNormal3f(x * zr1, y * zr1, z1);
                                glVertex3f(x * zr1, y * zr1, z1);
                            }
                        }
                        glEnd();
                        glEndList();
                        glListRecreate = false;
                    }
                    else
                        glCallList(glListCursor);
                    glPopMatrix();
                }

            }


            //Debug
            if(false) {
                glColor4f(0, 0, 0, 1);
                glBegin(GL_LINE_STRIP);
                glVertex3f(cursorPoly.at(1).x(), cursorPoly.at(1).y(), cursorPoly.at(1).z());
                glVertex3f(cursorPoly.at(2).x(), cursorPoly.at(2).y(), cursorPoly.at(2).z());
                glEnd();
                glBegin(GL_LINE_STRIP);
                glVertex3f(cursorPolyOld.at(1).x(), cursorPolyOld.at(1).y(), cursorPolyOld.at(1).z());
                glVertex3f(cursorPolyOld.at(2).x(), cursorPolyOld.at(2).y(), cursorPolyOld.at(2).z());
                glEnd();
            }

            //Mapping area
            if((selectedHover) || (selected)) {
                if((boundsSourceMode == 2) || (!curve)) {
                    boundsSource = Render::axisArea;
                    boundsSource.translate(-Render::axisCenter);
                }
                glPushMatrix();
                glColor4f(color.redF(), color.greenF(), color.blueF(), color.alphaF() / 2.F);
                glLineWidth(OpenGlDrawing::dpi);
                glEnable(GL_LINE_STIPPLE);
                glLineStipple(1, 255);
                glBegin(GL_LINE_LOOP);
                glVertex3f(boundsSource.topLeft().x(),     boundsSource.topLeft().y(),     boundsSource.topLeft().z());
                glVertex3f(boundsSource.topRight().x(),    boundsSource.topRight().y(),    boundsSource.topRight().z());
                glVertex3f(boundsSource.bottomRight().x(), boundsSource.bottomRight().y(), boundsSource.topRight().z());
                glVertex3f(boundsSource.bottomLeft().x(),  boundsSource.bottomLeft().y(),  boundsSource.topLeft().z());
                glEnd();
                if(boundsSource.length() != 0) {
                    glBegin(GL_LINE_LOOP);
                    glVertex3f(boundsSource.topLeft().x(),     boundsSource.topLeft().y(),     boundsSource.bottomLeft().z());
                    glVertex3f(boundsSource.topRight().x(),    boundsSource.topRight().y(),    boundsSource.bottomRight().z());
                    glVertex3f(boundsSource.bottomRight().x(), boundsSource.bottomRight().y(), boundsSource.bottomRight().z());
                    glVertex3f(boundsSource.bottomLeft().x(),  boundsSource.bottomLeft().y(),  boundsSource.bottomLeft().z());
                    glEnd();
                    glBegin(GL_LINES);
                    glVertex3f(boundsSource.topLeft().x(),     boundsSource.topLeft().y(),     boundsSource.topLeft().z());
                    glVertex3f(boundsSource.topLeft().x(),     boundsSource.topLeft().y(),     boundsSource.bottomLeft().z());
                    glVertex3f(boundsSource.topRight().x(),    boundsSource.topRight().y(),    boundsSource.topRight().z());
                    glVertex3f(boundsSource.topRight().x(),    boundsSource.topRight().y(),    boundsSource.bottomRight().z());
                    glVertex3f(boundsSource.bottomRight().x(), boundsSource.bottomRight().y(), boundsSource.topRight().z());
                    glVertex3f(boundsSource.bottomRight().x(), boundsSource.bottomRight().y(), boundsSource.bottomRight().z());
                    glVertex3f(boundsSource.bottomLeft().x(),  boundsSource.bottomLeft().y(),  boundsSource.topLeft().z());
                    glVertex3f(boundsSource.bottomLeft().x(),  boundsSource.bottomLeft().y(),  boundsSource.bottomLeft().z());
                    glEnd();
                }
                glDisable(GL_LINE_STIPPLE);
                Application::render->renderText(boundsSource.topLeft().x()     - 0.30, boundsSource.topLeft().y()     + 0.30, boundsSource.topLeft().z(),   QString::number(boundsTarget.topLeft().y(),     'f', 3), Application::renderFont, true);
                Application::render->renderText(boundsSource.bottomLeft().x()  - 0.60, boundsSource.bottomLeft().y()  + 0.30, boundsSource.topLeft().z(),   QString::number(boundsTarget.bottomLeft().y(),  'f', 3), Application::renderFont, true);
                Application::render->renderText(boundsSource.bottomLeft().x()  - 0.00, boundsSource.bottomLeft().y()  - 0.15, boundsSource.topLeft().z(),   QString::number(boundsTarget.bottomLeft().x(),  'f', 3), Application::renderFont, true);
                Application::render->renderText(boundsSource.bottomRight().x() - 0.30, boundsSource.bottomRight().y() - 0.15, boundsSource.topRight().z(),  QString::number(boundsTarget.bottomRight().x(), 'f', 3), Application::renderFont, true);
                if(boundsSource.length() != 0) {
                    Application::render->renderText(boundsSource.center().x() - 0.40, boundsSource.center().y() - 0.22, boundsSource.bottomRight().z(),     QString::number(boundsTarget.bottomRight().z(), 'f', 3), Application::renderFont, true);
                    Application::render->renderText(boundsSource.center().x() - 0.40, boundsSource.center().y() - 0.22, boundsSource.topRight().z() - 0.50, QString::number(boundsTarget.topRight().z(),    'f', 3), Application::renderFont, true);
                }
                glPopMatrix();
            }
        }
    }
}

void NxCursor::trig(bool force) {
    if((force) || ((((previousCursorReliable) && (hasActivity)) || (!curve)) && (canSendOsc()))) {
        MessageManager::outgoingMessage(MessageManagerDestination(this, 0, this));
        cursorPosLastSend         = cursorPos;
        cursorAedLastSend         = cursorAed;
        cursorRelativePosLastSend = cursorRelativePos;
        cursorRelativeAedLastSend = cursorRelativeAed;
        cursorAngleLastSend       = cursorAngle;
        timeLocalLastSend         = timeLocal;
        timeLastSend              = time;
        incMessageId();
    }
}

bool NxCursor::contains(NxTrigger *trigger) const {
    qint64 timestamp = Transport::currentMSecsSinceEpoch;
    if((previousPreviousCursorReliable) && (trigger->getActive()) && (!trigger->cursorTrigged)/* && ((timestamp - trigger->lastTrigTime) > 0)*/) {
        NxPoint centre1 = trigger->getPos() - NxPoint(  (cursorPoly.at(0).x() + cursorPoly.at(1).x() + cursorPoly.at(2).x() + cursorPoly.at(3).x()) / 4.,
                                                        (cursorPoly.at(0).y() + cursorPoly.at(1).y() + cursorPoly.at(2).y() + cursorPoly.at(3).y()) / 4.,
                                                        (cursorPoly.at(0).z() + cursorPoly.at(1).z() + cursorPoly.at(2).z() + cursorPoly.at(3).z()) / 4.);
        NxPoint centre2 = trigger->getPos() - NxPoint(  (cursorPolyOldOld.at(0).x() + cursorPolyOldOld.at(1).x() + cursorPolyOldOld.at(2).x() + cursorPolyOldOld.at(3).x()) / 4.,
                                                        (cursorPolyOldOld.at(0).y() + cursorPolyOldOld.at(1).y() + cursorPolyOldOld.at(2).y() + cursorPolyOldOld.at(3).y()) / 4.,
                                                        (cursorPolyOldOld.at(0).z() + cursorPolyOldOld.at(1).z() + cursorPolyOldOld.at(2).z() + cursorPolyOldOld.at(3).z()) / 4.);
        //Rotations + translations
        qreal angleSin, angleCos;
        //angle = -cursorAngle.z() * M_PI / 180., angleSin = qSin(angle), angleCos = qCos(angle);
        angleSin = cursorAngleCacheSinZ;
        angleCos = cursorAngleCacheCosZ;
        centre1 = NxPoint(centre1.x() * angleCos - centre1.y() * angleSin,
                          centre1.x() * angleSin + centre1.y() * angleCos,
                          centre1.z());
        centre2 = NxPoint(centre2.x() * angleCos - centre2.y() * angleSin,
                          centre2.x() * angleSin + centre2.y() * angleCos,
                          centre2.z());
        //angle = -cursorAngle.y() * M_PI / 180., angleSin = qSin(angle), angleCos = qCos(angle);
        angleSin = cursorAngleCacheSinY;
        angleCos = cursorAngleCacheCosY;
        centre1 = NxPoint(centre1.z() * angleSin + centre1.x() * angleCos,
                          centre1.y(),
                          centre1.z() * angleCos - centre1.x() * angleSin);
        centre2 = NxPoint(centre2.z() * angleSin + centre2.x() * angleCos,
                          centre2.y(),
                          centre2.z() * angleCos - centre2.x() * angleSin);

        /*
        qDebug("%f %f %f", centre1.x(), centre1.y(), centre1.z());
        qDebug("%f %f %f", centre2.x(), centre2.y(), centre2.z());
        qDebug("--------------------------------------------------------");
        */

        bool isInWidth = (qAbs(centre1.y()) <= width/2.) && (qAbs(centre2.y()) <= width/2.);
        bool isInDepth = (qAbs(centre1.z()) <= depth/2.) && (qAbs(centre2.z()) <= depth/2.);
        bool isInside  = ((centre1.x() >= 0) && (centre2.x() <= 0)) || ((centre1.x() <= 0) && (centre2.x() >= 0));

        if(depth > 0) {
            if(isInDepth && isInWidth && isInside) {
                //qDebug("A > %f %d %d %d", timeLocal, isInDepth, isInWidth, isInside);
                //qDebug("%d\t=\t%f\t%f => BY %d", isInside, centre1.x(), centre2.x(), trigger->cursorTrigged);
                trigger->lastTrigTime = timestamp;
                return true;
            }
            else
                return false;
        }
        else {
            if(isInWidth && isInside) {
                trigger->lastTrigTime = timestamp;
                return true;
            }
            else
                return false;
        }
    }
    else
        return false;
    /*
    if((previousCursorReliable) && (trigger->getActive()) && ((trigger->getPos().z()-depth/2 <= cursorPoly.at(0).z()) && (cursorPoly.at(0).z()) <= trigger->getPos().z()+depth/2) && (cursorPoly.containsPoint(trigger->getPos(), Qt::OddEvenFill)))
        return true;
    else
        return false;
        */
}
bool NxCursor::trig(NxCurve *collisionCurve) {
    if((performCollision) && (collisionCurve) && (collisionCurve->getActive()) && (collisionCurve != curve)) {
        NxPoint collisionPoint;
        qreal percent = collisionCurve->intersects(boundingRect, &collisionPoint);
        if(percent >= 0) {
            MessageManager::outgoingMessage(MessageManagerDestination(this, 0, this, collisionCurve, collisionPoint, getCursorValue(collisionPoint)));
            return true;
        }
        return false;
    }
    else
        return false;
}


void NxCursor::setBoundsRect(quint16 index, qreal val, bool source) {
    NxRect *boundsRect;
    if(source) {
        boundsRect = &boundsSource;
        boundsSourceMode = 3;
    }
    else
        boundsRect = &boundsTarget;
    if(     index == 0) boundsRect->setTopLeft    (NxPoint(val,                           boundsRect->topLeft()    .y(), boundsRect->topLeft()    .z()));
    else if(index == 1) boundsRect->setTopLeft    (NxPoint(boundsRect->topLeft()    .x(), val,                           boundsRect->topLeft()    .z()));
    else if(index == 5) boundsRect->setTopLeft    (NxPoint(boundsRect->topLeft()    .x(), boundsRect->topLeft()    .y(), val));
    else if(index == 3) boundsRect->setBottomRight(NxPoint(val,                           boundsRect->bottomRight().y(), boundsRect->bottomRight().z()));
    else if(index == 4) boundsRect->setBottomRight(NxPoint(boundsRect->bottomRight().x(), val,                           boundsRect->bottomRight().z()));
    else if(index == 2) boundsRect->setBottomRight(NxPoint(boundsRect->bottomRight().x(), boundsRect->bottomRight().y(), val));
}
qreal NxCursor::getBoundsRect(quint16 index, bool source) const {
    NxRect boundsRect;
    if(source)  boundsRect = boundsSource;
    else        boundsRect = boundsTarget;
    if(index == 0)      return boundsRect.topLeft().x();
    else if(index == 1) return boundsRect.topLeft().y();
    else if(index == 2) return boundsRect.topLeft().z();
    else if(index == 3) return boundsRect.bottomRight().x();
    else if(index == 4) return boundsRect.bottomRight().y();
    else if(index == 5) return boundsRect.bottomRight().z();
    return 0;
}

void NxCursor::setBoundsRectStr(const QString & _bounds, bool source) {
    QStringList bounds = _bounds.split(" ", QString::SkipEmptyParts);
    if(bounds.count() == 4) {
        setBoundsRect(0, bounds.at(0).toDouble(), source);
        setBoundsRect(3, bounds.at(1).toDouble(), source);
        setBoundsRect(4, bounds.at(2).toDouble(), source);
        setBoundsRect(1, bounds.at(3).toDouble(), source);
        setBoundsRect(2, 0, source);
        setBoundsRect(5, 0, source);
    }
    else if(bounds.count() == 6) {
        setBoundsRect(0, bounds.at(0).toDouble(), source);
        setBoundsRect(3, bounds.at(1).toDouble(), source);
        setBoundsRect(4, bounds.at(2).toDouble(), source);
        setBoundsRect(1, bounds.at(3).toDouble(), source);
        setBoundsRect(2, bounds.at(4).toDouble(), source);
        setBoundsRect(5, bounds.at(5).toDouble(), source);
    }
    else if(bounds.count() > 1)
        setBoundsRect(bounds.at(0).toDouble(), bounds.at(1).toDouble(), source);
}
const QString NxCursor::getBoundsRectStr(bool source) const {
    return QString("%1 %2 %3 %4 %5 %6")
            .arg(getBoundsRect(0, source), 0, 'f', 3).arg(getBoundsRect(3, source), 0, 'f', 3)
            .arg(getBoundsRect(4, source), 0, 'f', 3).arg(getBoundsRect(1, source), 0, 'f', 3)
            .arg(getBoundsRect(2, source), 0, 'f', 3).arg(getBoundsRect(5, source), 0, 'f', 3);
}
