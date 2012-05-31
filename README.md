Adaptive Content
================

Drupal modules to load content asynchronously based on dimensions, context or
capabilities.

Callbacks (JS)
==============
When make Ajax calls for regions it should be done using the post method. This
is because form and the like uses the URL to make submissions to, so the
adaptive content parameters are parsed back when the form is submitted. Which
WILL break the form.

But for testing you can use the get method:

Load all regions and return them as a JSON array for the preset.
* node/1?responsive=1&preset=preset_1

Load all regions in the parameter "regions" and return them as a JSON array.
* node/1?responsive=1&regions=menu,preface_first

Patches
=======
If you are using the Omega theme as our base theme you need to patch the alpha
module supplied with the omaga theme with the file "alpha.patch". Copy the patch
to sites/all/themes/omega/alpha and execute:

patch -p1 < alpha.patch