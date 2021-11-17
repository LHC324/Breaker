# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
# import os
# import sys
# sys.path.insert(0, os.path.abspath('.'))


# -- Project information -----------------------------------------------------

project = '高压断路器动特性测试仪'
copyright = '2021, LHC@云南兆富科技有限公司'
author = 'LHC@云南兆富科技有限公司'

# The full version, including alpha/beta/rc tags
release = 'V1.0.0'


# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
]

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# The language for content autogenerated by Sphinx. Refer to documentation
# for a list of supported languages.
#
# This is also used if you do content translation via gettext catalogs.
# Usually you set "language" from the command line for these cases.
language = 'zh_CN'

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = []


# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme = 'sphinx_rtd_theme'

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']

# -- Options for LaTeX output ---------------------------------------------
latex_engine = 'xelatex'
latex_elements = {
'passoptionstopackages': r'''\PassOptionsToPackage{svgnames}{xcolor}''',
# The paper size ('letterpaper' or 'a4paper').
#'papersize': 'letterpaper',
'papersize': 'a4paper',

# The font size ('10pt', '11pt' or '12pt').
#'pointsize': '10pt',

# Additional stuff for the LaTeX preamble.
#'preamble': '',


# Latex figure (float) alignment
'figure_align': 'htbp',


'sphinxsetup': r'''
verbatimwithframe = true,
VerbatimColor = {named}{Gainsboro}, % background colour for code-blocks
VerbatimBorderColor = {rgb}{0.5,0.3,0.9}, % The frame color
VerbatimHighlightColor = {rgb}{0.878,1,1}, % The color for highlighted lines.
InnerLinkColor = {rgb}{0.208,0.374,0.486}, % Inner Link Color
OuterLinkColor = {named}{LightSkyBlue}, % Inner Link Color
TitleColor = {named}{Black},
hintBorderColor = {named}{Green},
dangerBorderColor = {named}{Red},
dangerBgColor = {named}{Tomato},
errorBorderColor = {named}{Crimson},
warningBorderColor = {named}{Chocolate},
attentionborder = 2pt,
attentionBorderColor = {named}{Salmon},
attentionBgColor = {named}{LightSalmon},
noteborder = 2pt,
noteBorderColor = {named}{Goldenrod},
importantborder = 2pt,
importantBorderColor = {named}{OrangeRed},
cautionborder = 2pt,
cautionBorderColor = {named}{Pink},
cautionBgColor = {named}{LightPink}''',

# Using Package for ZH
'preamble':r'''\usepackage{ctex}
\usepackage{bm}
''',
}