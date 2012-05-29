adaptivecontent
===============

Drupal modules to load content asynchronously based on dimensions, context or capabilities.

Patches
=======
If you are using the Omega theme as our base theme you need to patch the alpha
module supplied with the omaga theme with the file "alpha.patch". Copy the patch 
to sites/all/themes/omega/alpha and execute:

patch -p1 < alpha.patch 