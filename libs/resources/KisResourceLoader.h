/*
 * Copyright (C) 2018 Boudewijn Rempt <boud@valdyas.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef KISRESOURCELOADER_H
#define KISRESOURCELOADER_H

#include <QString>
#include <QStringList>
#include <QImage>
#include <QScopedPointer>
#include <QSharedPointer>

#include <KoResource.h>

#include <kritaresources_export.h>


/**
 * @brief The KisResourceLoader class is an abstract interface
 * class that must be implemented by actual resource classes and
 * registered with the KisResourceLoaderRegistry.
 */
class KRITARESOURCES_EXPORT KisResourceLoaderBase
{
public:

    KisResourceLoaderBase(const QString &id, const QString &folder, const QStringList &mimetypes)
    {
        m_id = id;
        m_folder = folder;
        m_mimetypes = mimetypes;
    }

    virtual ~KisResourceLoaderBase()
    {
    }

    /**
     * @return the mimetypes this resource can load
     */
    QStringList mimetypes() const
    {
        return m_mimetypes;
    }

    /**
     * @return the folder in the resource storage where resources
     * of this type are located
     */
    QString folder() const
    {
        return m_folder;
    }

    QString resourceType() const
    {
        return id();
    }

    /// For registration in KisResourceLoaderRegistry
    QString id() const
    {
        return m_id;
    }

    /**
     * Load this resource.
     * @return true if loading the resource succeeded.
     */
    virtual KoResourceSP load(const QString &name, QIODevice &dev) { Q_UNUSED(name); Q_UNUSED(dev); return 0; };

private:
    QString m_id;
    QString m_folder;
    QStringList m_mimetypes;

};

template<typename T>
class KRITARESOURCES_EXPORT KisResourceLoader : public KisResourceLoaderBase {
public:
    KisResourceLoader(const QString &id, const QString &folder, const QStringList &mimetypes)
        : KisResourceLoaderBase(id, folder, mimetypes)
    {
    }

    KoResourceSP load(const QString &name, QIODevice &dev) override
    {
        QSharedPointer<T> resource = QSharedPointer<T>::create(name);
        if (resource->loadFromDevice(&dev)) {
            return resource;
        }
        return 0;
    }
};



#endif // KISRESOURCELOADER_H
