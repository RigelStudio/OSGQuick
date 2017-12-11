/****************************************************************************
**
** Copyright (c) 2016, Leon Manukyan
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
**
** 1. Redistributions of source code must retain the above copyright
**    notice, this list of conditions and the following disclaimer.
**
** 2. Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in
**    the documentation and/or other materials provided with the
**    distribution.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
** TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
** PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
** LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
** NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
****************************************************************************/

#ifndef QTQUICK2_OSGMODELLOADER_HPP
#define QTQUICK2_OSGMODELLOADER_HPP


#include "osgitem.hpp"

class OSGModelLoader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString modelPath READ modelPath WRITE setModelPath NOTIFY modelPathChanged)
    Q_PROPERTY(OSGItem* target READ target WRITE setTarget NOTIFY targetChanged)

public: // types
    typedef OSGItem::ModelPtr ModelPtr;

public: // functions
    OSGModelLoader();

    QString modelPath() const;
    void setModelPath(QString modelPath);

    OSGItem* target() const;
    void setTarget(OSGItem* target);

    static ModelPtr loadModel(std::string modelName);

signals:
    void modelPathChanged();
    void targetChanged();

private: // functions
    void notifyTarget();

private: // data
    QString modelPath_;
    OSGItem* target_;
    ModelPtr model_;
};

#endif // QTQUICK2_OSGMODELLOADER_HPP
