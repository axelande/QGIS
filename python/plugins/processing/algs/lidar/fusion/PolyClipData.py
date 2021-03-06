# -*- coding: utf-8 -*-

"""
***************************************************************************
    PolyClipData.py
    ---------------------
    Date                 : May 2014
    Copyright            : (C) 2014 by Niccolo' Marchi
    Email                : sciurusurbanus at hotmail dot it
***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************
"""
from future import standard_library
standard_library.install_aliases()
from builtins import str

__author__ = "Niccolo' Marchi"
__date__ = 'May 2014'
__copyright__ = "(C) 2014 by Niccolo' Marchi"

# This will get replaced with a git SHA1 when you do a git archive

__revision__ = '$Format:%H$'

import os
from processing.core.parameters import ParameterFile
from processing.core.parameters import ParameterBoolean
from processing.core.parameters import ParameterString
from processing.core.outputs import OutputFile
from .FusionAlgorithm import FusionAlgorithm
from .FusionUtils import FusionUtils


class PolyClipData(FusionAlgorithm):

    INPUT = 'INPUT'
    OUTPUT = 'OUTPUT'
    SHAPE = 'SHAPE'
    MASK = 'MASK'
    FIELD = 'FIELD'
    VALUE = 'VALUE'
    MULTIFILE = 'MULTIFILE'

    def defineCharacteristics(self):
        self.name, self.i18n_name = self.trAlgorithm('Poly Clip Data')
        self.group, self.i18n_group = self.trAlgorithm('Points')
        self.addParameter(ParameterFile(
            self.INPUT, self.tr('Input LAS layer'),
            optional=False))
        self.addParameter(ParameterFile(self.MASK, self.tr('Mask layer'), optional=False))
        self.addOutput(OutputFile(self.OUTPUT,
                                  self.tr('Output clipped LAS file'), 'las'))
        self.addParameter(ParameterString(self.SHAPE, self.tr("Use Shape attribute (shp column number,value)"), optional=True))
        self.addParameter(ParameterString(self.FIELD,
                                          self.tr('Shape field index')))
        self.addParameter(ParameterString(self.VALUE, self.tr("Shape value")))
        self.addParameter(ParameterBoolean(self.MULTIFILE, self.tr('Create a file per each polygon'), False))
        self.addAdvancedModifiers()

    def processAlgorithm(self, feedback):
        commands = [os.path.join(FusionUtils.FusionPath(), 'PolyClipData.exe')]
        commands.append('/verbose')
        if self.getParameterValue(self.SHAPE):
            commands.append('/shape:' + str(self.getParameterValue(self.FIELD)) + ',' + str(self.getParameterValue(self.VALUE)))
        multiFile = self.getParameterValue(self.MULTIFILE)
        if multiFile:
            commands.append('/multifile')
        self.addAdvancedModifiersToCommand(commands)
        commands.append(self.getParameterValue(self.MASK))
        outFile = self.getOutputValue(self.OUTPUT)
        commands.append(outFile)
        files = self.getParameterValue(self.INPUT).split(';')
        if len(files) == 1:
            commands.append(self.getParameterValue(self.INPUT))
        else:
            FusionUtils.createFileList(files)
            commands.append(FusionUtils.tempFileListFilepath())

        FusionUtils.runFusion(commands, feedback)
