#ifndef NODE_TAGLIB_H
#define NODE_TAGLIB_H

#include <fileref.h>
#include <tag.h>
#include <node.h>
#include <uv.h>
#ifdef _WINDOWS
#include <time.h>
#else
#include <sys/time.h>
#endif

#include <node_version.h>

namespace node_taglib {
class Tag;
class BufferStream;

/**
 * Note: This uses TagLib's internal file type resolvers
 * use CreateFileRef with a FileStream if the format is known
 */
int CreateFileRefPath(TagLib::FileName path, TagLib::FileRef **ref);
int CreateFileRef(TagLib::IOStream *stream, TagLib::String format, TagLib::FileRef **ref);
static TagLib::File *createFile(TagLib::IOStream *stream, TagLib::String format);
v8::Handle<v8::String> ErrorToString(int error);
v8::Handle<v8::Value> TagLibStringToString( TagLib::String s );
v8::Handle<v8::Value> TagLibStringToString( TagLib::FileName s );
TagLib::String NodeStringToTagLibString( v8::Local<v8::Value> s );
v8::Handle<v8::Value> AsyncReadFile(const v8::Arguments &args);
void AsyncReadFileDo(uv_work_t *req);
void AsyncReadFileAfter(uv_work_t *req);

struct AsyncBaton {
    AsyncBaton(TagLib::FileName p) : path(TagLib::String::null.toCString()) {};
    uv_work_t request;
    v8::Persistent<v8::Function> callback;
    int error;

    TagLib::FileName path; /* only used by read/tag, not save */
    // OR
    TagLib::String format;
    BufferStream *stream; // File takes over ownership of the stream
                          // and FileRef takes over ownership of the File
                          // so don't do BufferStream deletion

    TagLib::FileRef *fileRef; /* only used by taglib.read */
    Tag *tag; /* only used by taglib.tag */
};

v8::Handle<v8::Value> AddResolvers(const v8::Arguments &args);

class CallbackResolver;

struct AsyncResolverBaton {
    AsyncResolverBaton(TagLib::FileName f) : fileName(TagLib::String::null.toCString()) {};
    uv_async_t request;
    const CallbackResolver *resolver;
    TagLib::FileName fileName;
    TagLib::String type;
    uv_async_t idler;
};

class CallbackResolver : public TagLib::FileRef::FileTypeResolver {
    v8::Persistent<v8::Function> resolverFunc;
    #ifdef _WINDOWS
    const unsigned long created_in;
    #else
    const uv_thread_t created_in;
    #endif

public:
    CallbackResolver(v8::Persistent<v8::Function> func);
    TagLib::File *createFile(TagLib::FileName fileName, bool readAudioProperties, TagLib::AudioProperties::ReadStyle audioPropertiesStyle) const;
    static void invokeResolverCb(uv_async_t *handle, int status);
    static void stopIdling(uv_async_t *handle, int status);
    static void invokeResolver(AsyncResolverBaton *baton);
};

}
#endif
