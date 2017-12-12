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

#include "osgmodelloader.hpp"

#include <osgDB/ReadFile>

OSGModelLoader::OSGModelLoader()
    : target_(NULL)
{
}

QString
OSGModelLoader::modelPath() const
{
    return modelPath_;
}

void
OSGModelLoader::setModelPath(QString modelPath)
{
    modelPath_ = modelPath;
    emit modelPathChanged();
    notifyTarget();
}

OSGItem*
OSGModelLoader::target() const
{
    return target_;
}

void
OSGModelLoader::setTarget(OSGItem* target)
{
    target_ = target;
    emit targetChanged();
    notifyTarget();
}

OSGModelLoader::ModelPtr
OSGModelLoader::loadModel(std::string modelFile)
{
    if ( modelFile.substr(0, 8) == "file:///" )
        modelFile = modelFile.substr(8);

    const ModelPtr loadedModel = osgDB::readNodeFile(modelFile);
    if (!loadedModel)
    {
        std::cerr << "Error: Could not load '" << modelFile << "'." << std::endl;
    }

    return loadedModel;
}

void
OSGModelLoader::notifyTarget()
{
    if ( target_ && !modelPath_.isEmpty() )
    {
        target_->setModel(loadModel(modelPath_.toStdString()));
    }
}
