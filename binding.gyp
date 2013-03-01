{
  "targets": [
    {
      "target_name": "taglib",
      "sources": ["src/bufferstream.c", "src/tag.cc", "src/taglib.cc"],
      
      'conditions': [
        ['OS=="mac"', {
          # cflags on OS X are stupid and have to be defined like this
          # copied from libxmljs
          'xcode_settings': {
            'OTHER_CFLAGS': [
              '<!@(taglib-config --cflags)'
            ],
            'OTHER_LDFLAGS': [
              '-dynamiclib'
            ]
          }
        }, {
          
        }],
        ['OS=="win"', {
          'libraries': ['-LC:/taglib/lib -ltag'],
          'defines': [
            '_WINDOWS',
            '_WINSOCKAPI_',
            # to avoid problems with winsock2 inclusion in windows.h
            'WIN32_LEAN_AND_MEAN'
          ],
          'include_dirs': ['C:/taglib/include/taglib']
        }, { # OS!="win"
          'libraries': ['<!(taglib-config --libs)'],
          'cflags': [
            '<!@(taglib-config --cflags)'
          ]
        }]
      ]
    }
  ]
}
