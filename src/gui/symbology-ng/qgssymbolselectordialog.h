/***************************************************************************
    qgssymbolselectordialog.h
    ---------------------
    begin                : November 2009
    copyright            : (C) 2009 by Martin Dobias
    email                : wonder dot sk at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSSYMBOLSELECTORDIALOG_H
#define QGSSYMBOLSELECTORDIALOG_H

#include <QDialog>

#include "ui_qgssymbolselectordialogbase.h"

#include "qgspanelwidget.h"
#include "qgssymbolwidgetcontext.h"
#include "qgsproperty.h"

#include <QStandardItemModel>
#include <QScopedPointer>
#include <QDialogButtonBox>
#include "qgis_gui.h"

class QgsStyle;
class QgsSymbol;
class QgsSymbolLayer;
class QgsVectorLayer;

class QMenu;
class QWidget;

class SymbolLayerItem;
class QgsMarkerSymbol;
class QgsLineSymbol;
class QgsMarkerSymbolLayer;
class QgsLineSymbolLayer;

class QgsMapCanvas;

/// @cond PRIVATE

class DataDefinedRestorer: public QObject
{
    Q_OBJECT
  public:
    DataDefinedRestorer( QgsSymbol* symbol, const QgsSymbolLayer* symbolLayer );

  public slots:
    void restore();

  private:
    QgsMarkerSymbol* mMarker;
    const QgsMarkerSymbolLayer* mMarkerSymbolLayer;
    double mSize;
    double mAngle;
    QPointF mMarkerOffset;
    QgsProperty mDDSize;
    QgsProperty mDDAngle;

    QgsLineSymbol* mLine;
    const QgsLineSymbolLayer* mLineSymbolLayer;
    double mWidth;
    double mLineOffset;
    QgsProperty mDDWidth;

    void save();
};
///@endcond

class QgsSymbolSelectorDialog;

/** \ingroup gui
 * Symbol selector widget that cna be used to select and build a symbol
 */
class GUI_EXPORT QgsSymbolSelectorWidget: public QgsPanelWidget, private Ui::QgsSymbolSelectorDialogBase
{
    Q_OBJECT
    /// Too allow for non API break access from the dialog.
    friend class QgsSymbolSelectorDialog;

  public:

    /**
       * Symbol selector widget that can be used to select and build a symbol
       * @param symbol The symbol to load into the widget as a start point.
       * @param style The style used by the widget.
       * @param vl The vector layer for the symbol.
       * @param parent
       */
    QgsSymbolSelectorWidget( QgsSymbol* symbol, QgsStyle* style, const QgsVectorLayer* vl, QWidget* parent = nullptr );

    //! return menu for "advanced" button - create it if doesn't exist and show the advanced button
    QMenu* advancedMenu();

    /** Sets the context in which the symbol widget is shown, e.g., the associated map canvas and expression contexts.
     * @param context symbol widget context
     * @see context()
     * @note added in QGIS 3.0
     */
    void setContext( const QgsSymbolWidgetContext& context );

    /** Returns the context in which the symbol widget is shown, e.g., the associated map canvas and expression contexts.
     * @see setContext()
     * @note added in QGIS 3.0
     */
    QgsSymbolWidgetContext context() const;

    /**
     * @brief Return the symbol that is currently active in the widget. Can be null.
     * @return The active symbol.
     */
    QgsSymbol* symbol() { return mSymbol; }

  protected:

    /**
     * Reload the current symbol in the view.
     */
    void loadSymbol();
    //! @note not available in python bindings

    /**
     * Load the given symbol into the widget..
     * @param symbol The symbol to load.
     * @param parent The parent symbol layer item.
     */
    void loadSymbol( QgsSymbol* symbol, SymbolLayerItem* parent );

    /**
     * Update the state of the UI based on the currently set symbol layer.
     */
    void updateUi();

    /**
     * Update the lock button states based on the current symbol layer.
     */
    void updateLockButton();

    //! @note not available in python bindings
    SymbolLayerItem* currentLayerItem();

    /**
     * The current symbol layer that is active in the interface.
     * @return The active symbol layer.
     */
    QgsSymbolLayer* currentLayer();

    /**
     * Move the current active layer by a set offset in the list.
     * @param offset The offset to move the layer by
     */
    void moveLayerByOffset( int offset );

    /**
     * Set the properties widget for the active symbol layer.
     * @param widget The widget to set to configure the active symbol layer.
     */
    void setWidget( QWidget* widget );

  signals:

    /**
     * Emiited when a symbol is modified in the widget.
     */
    void symbolModified();

  public slots:

    /**
     * Move the active symbol layer down.
     */
    void moveLayerDown();

    /**
     * Move the active symbol layer up.
     */
    void moveLayerUp();

    /**
     * Add a symobl layer to the bottom of the stack.
     */
    void addLayer();

    /**
     * Remove the current active symbol layer.
     */
    void removeLayer();

    /**
     * Lock the current active symbol layer.
     */
    void lockLayer();

    //! Duplicates the current symbol layer and places the duplicated layer above the current symbol layer
    //! @note added in QGIS 2.14
    void duplicateLayer();

    /**
     * Called when the layer changes in the widget. Updates the active properties for
     * active symbol layer.
     */
    void layerChanged();

    /**
     * Update the single symbol layer preview in the widget.
     */
    void updateLayerPreview();

    /**
     * Update the preview of the whole symbol in the interface.
     */
    void updatePreview();

    //! Slot to update tree when a new symbol from style
    void symbolChanged();
    //! alters tree and sets proper widget when Layer Type is changed
    //! @note: The layer is received from the LayerPropertiesWidget
    void changeLayer( QgsSymbolLayer* layer );


  protected: // data
    QgsStyle* mStyle;
    QgsSymbol* mSymbol;
    QMenu* mAdvancedMenu;
    const QgsVectorLayer* mVectorLayer;

    QStandardItemModel* model;
    QWidget *mPresentWidget;

  private:
    QScopedPointer<DataDefinedRestorer> mDataDefineRestorer;
    QgsSymbolWidgetContext mContext;
};

/** \ingroup gui
 * \class QgsSymbolSelectorDialog
 */
class GUI_EXPORT QgsSymbolSelectorDialog : public QDialog
{
    Q_OBJECT

  public:
    QgsSymbolSelectorDialog( QgsSymbol* symbol, QgsStyle* style, const QgsVectorLayer* vl, QWidget* parent = nullptr, bool embedded = false );
    ~QgsSymbolSelectorDialog();

    //! return menu for "advanced" button - create it if doesn't exist and show the advanced button
    QMenu* advancedMenu();

    /** Sets the context in which the symbol widget is shown, e.g., the associated map canvas and expression contexts.
     * @param context symbol widget context
     * @see context()
     * @note added in QGIS 3.0
     */
    void setContext( const QgsSymbolWidgetContext& context );

    /** Returns the context in which the symbol widget is shown, e.g., the associated map canvas and expression contexts.
     * @see setContext()
     * @note added in QGIS 3.0
     */
    QgsSymbolWidgetContext context() const;

    /**
     * @brief Return the symbol that is currently active in the widget. Can be null.
     * @return The active symbol.
     */
    QgsSymbol* symbol();

  protected:
    // Reimplements dialog keyPress event so we can ignore it
    void keyPressEvent( QKeyEvent * e ) override;

    void loadSymbol();
    //! @note not available in python bindings
    void loadSymbol( QgsSymbol* symbol, SymbolLayerItem* parent );

    void updateUi();

    void updateLockButton();

    //! @note not available in python bindings
    SymbolLayerItem* currentLayerItem();
    QgsSymbolLayer* currentLayer();

    void moveLayerByOffset( int offset );

    void setWidget( QWidget* widget );

  signals:
    void symbolModified();

  public slots:
    void moveLayerDown();
    void moveLayerUp();

    void addLayer();
    void removeLayer();

    void lockLayer();

    //! Duplicates the current symbol layer and places the duplicated layer above the current symbol layer
    //! @note added in QGIS 2.14
    void duplicateLayer();

    void layerChanged();

    void updateLayerPreview();
    void updatePreview();

    //! Slot to update tree when a new symbol from style
    void symbolChanged();
    //! alters tree and sets proper widget when Layer Type is changed
    //! @note: The layer is received from the LayerPropertiesWidget
    void changeLayer( QgsSymbolLayer* layer );

  private:
    QgsSymbolSelectorWidget* mSelectorWidget;
    QDialogButtonBox* mButtonBox;
    QgsSymbolWidgetContext mContext;
};

#endif
