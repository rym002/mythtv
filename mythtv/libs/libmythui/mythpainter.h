#ifndef MYTHPAINTER_H_
#define MYTHPAINTER_H_

#include <QMap>
#include <QString>
#include <QTextLayout>
#include <QWidget>
#include <QPaintDevice>
#include <QMutex>
#include <QSet>

class QRect;
class QRegion;
class QPoint;
class QColor;

#include "mythuiexp.h"

#include <list>

#ifdef _MSC_VER
#  include <cstdint>    // int64_t
#endif

class MythFontProperties;
class MythImage;
class UIEffects;

using LayoutVector = QVector<QTextLayout *>;
using FormatVector = QVector<QTextLayout::FormatRange>;

class MUI_PUBLIC MythPainter
{
  public:
    MythPainter();
    /** MythPainter destructor.
     *
     *  The MythPainter destructor does not cleanup, it is unsafe
     *  to do cleanup in the MythPainter destructor because
     *  DeleteImagePriv() is a pure virtual in this class. Instead
     *  children should call MythPainter::Teardown() for cleanup.
     */
    virtual ~MythPainter() = default;

    virtual QString GetName(void) = 0;
    virtual bool SupportsAnimation(void) = 0;
    virtual bool SupportsAlpha(void) = 0;
    virtual bool SupportsClipping(void) = 0;
    virtual void FreeResources(void) { }
    virtual void Begin(QPaintDevice *parent) { m_parent = parent; }
    virtual void End() { m_parent = nullptr; }

    virtual void SetClipRect(const QRect &clipRect);
    virtual void SetClipRegion(const QRegion &clipRegion);
    virtual void Clear(QPaintDevice *device, const QRegion &region);

    QPaintDevice *GetParent(void) { return m_parent; }

    virtual void DrawImage(const QRect &dest, MythImage *im, const QRect &src,
                           int alpha) = 0;

    void DrawImage(int x, int y, MythImage *im, int alpha);
    void DrawImage(const QPoint &topLeft, MythImage *im, int alph);

    virtual void DrawText(const QRect &r, const QString &msg, int flags,
                          const MythFontProperties &font, int alpha,
                          const QRect &boundRect);
    virtual void DrawTextLayout(const QRect &canvasRect,
                                const LayoutVector & layouts,
                                const FormatVector & formats,
                                const MythFontProperties &font, int alpha,
                                const QRect &destRect);
    virtual void DrawRect(const QRect &area, const QBrush &fillBrush,
                          const QPen &linePen, int alpha);
    virtual void DrawRoundRect(const QRect &area, int cornerRadius,
                               const QBrush &fillBrush, const QPen &linePen,
                               int alpha);
    virtual void DrawEllipse(const QRect &area, const QBrush &fillBrush,
                             const QPen &linePen, int alpha);

    virtual void PushTransformation(const UIEffects &zoom, QPointF center = QPointF());
    virtual void PopTransformation(void) { }

    /// Returns a blank reference counted image in the format required
    /// for the Draw functions for this painter.
    /// \note The reference count is set for one use, call DecrRef() to delete.
    MythImage *GetFormatImage();
    void DeleteFormatImage(MythImage *im);

    void SetDebugMode(bool showBorders, bool showNames)
    {
        m_showBorders = showBorders;
        m_showNames = showNames;
    }

    bool ShowBorders(void) { return m_showBorders; }
    bool ShowTypeNames(void) { return m_showNames; }

    void SetMaximumCacheSizes(int hardware, int software);

  protected:
    static void DrawTextPriv(MythImage *im, const QString &msg, int flags,
                             const QRect &r, const MythFontProperties &font);
    static void DrawRectPriv(MythImage *im, const QRect &area, int radius, int ellipse,
                             const QBrush &fillBrush, const QPen &linePen);

    MythImage *GetImageFromString(const QString &msg, int flags, const QRect &r,
                                  const MythFontProperties &font);
    MythImage *GetImageFromTextLayout(const LayoutVector & layouts,
                                      const FormatVector & formats,
                                      const MythFontProperties &font,
                                      QRect &canvas, QRect &dest);
    MythImage *GetImageFromRect(const QRect &area, int radius, int ellipse,
                                const QBrush &fillBrush,
                                const QPen &linePen);

    /// Creates a reference counted image, call DecrRef() to delete.
    virtual MythImage* GetFormatImagePriv(void) = 0;
    virtual void DeleteFormatImagePriv(MythImage *im) = 0;
    void ExpireImages(int64_t max = 0);

    // This needs to be called by classes inheriting from MythPainter
    // in the destructor.
    virtual void Teardown(void);

    void CheckFormatImage(MythImage *im);

    QPaintDevice *m_parent      {nullptr};
    int m_hardwareCacheSize     {0};
    int m_maxHardwareCacheSize  {0};

  private:
    int64_t m_softwareCacheSize {0};
    int64_t m_maxSoftwareCacheSize {1024 * 1024 * 48};

    QMutex           m_allocationLock;
    QSet<MythImage*> m_allocatedImages;

    QMap<QString, MythImage *> m_stringToImageMap;
    std::list<QString>         m_stringExpireList;

    bool m_showBorders          {false};
    bool m_showNames            {false};
};

#endif
