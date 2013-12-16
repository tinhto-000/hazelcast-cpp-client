#include "hazelcast/client/serialization/OutputSocketStream.h"
#include "hazelcast/client/Socket.h"
#include "hazelcast/client/exception/IOException.h"
#include <algorithm>

namespace hazelcast {
    namespace client {
        namespace serialization {


            OutputSocketStream::OutputSocketStream(Socket & socket)
            :socket(socket) {
            }

            void OutputSocketStream::write(const std::vector<byte>& bytes) {
                socket.send((void *) bytes.data(), sizeof (char) * bytes.size());
            };

            void OutputSocketStream::writeBoolean(bool i) {
                writeByte(i);
            };

            void OutputSocketStream::writeByte(int i) {
                char x = (char) (0xff & i);
                socket.send((void *) &(x), sizeof(char));
            };

            void OutputSocketStream::writeShort(int v) {
                writeByte((v >> 8));
                writeByte(v);
            };

            void OutputSocketStream::writeChar(int i) {
                writeByte((i >> 8));
                writeByte(i);
            };

            void OutputSocketStream::writeInt(int v) {
                writeByte((v >> 24));
                writeByte((v >> 16));
                writeByte((v >> 8));
                writeByte(v);
            };

            void OutputSocketStream::writeLong(long l) {
                writeByte((l >> 56));
                writeByte((l >> 48));
                writeByte((l >> 40));
                writeByte((l >> 32));
                writeByte((l >> 24));
                writeByte((l >> 16));
                writeByte((l >> 8));
                writeByte((int) l);
            };

            void OutputSocketStream::writeFloat(float x) {
                union {
                    float f;
                    int i;
                } u;
                u.f = x;
                writeInt(u.i);
            };

            void OutputSocketStream::writeDouble(double v) {
                union {
                    double d;
                    long l;
                } u;
                u.d = v;
                writeLong(u.l);
            };

            void OutputSocketStream::writeUTF(const std::string& str) {
                bool isNull = str.empty();
                writeBoolean(isNull);
                if (isNull)
                    return;

                int length = (int) str.length();
                writeInt(length);
                int chunkSize = length / STRING_CHUNK_SIZE + 1;
                for (int i = 0; i < chunkSize; i++) {
					using namespace std;//for stupid visual studio compatibility
                    int beginIndex = max(0, i * STRING_CHUNK_SIZE - 1);
                    int endIndex = min((i + 1) * STRING_CHUNK_SIZE - 1, length);
                    writeShortUTF(str.substr(beginIndex, endIndex - beginIndex));
                }
            };

            //private functions

            void OutputSocketStream::writeShortUTF(const std::string& str) {
                int stringLen = (int) str.length();
                int utfLength = 0;
                int count = 0;
                /* use charAt instead of copying String to char std::vector */
                for (int i = 0; i < stringLen; i++) {
                    if ((str[i] >= 0x0001) && (str[i] <= 0x007F)) {
                        utfLength++;
                    } else if (str[i] > 0x07FF) {
                        utfLength += 3;
                    } else {
                        utfLength += 2;
                    }
                }
                if (utfLength > 65535) {
                    throw exception::IOException("OutputSocketStream::writeShortUTF", "encoded string too long:" + util::to_string(utfLength) + " bytes");
                }
                std::vector<byte> byteArray(utfLength);
                int i;
                for (i = 0; i < stringLen; i++) {
                    if (!((str[i] >= 0x0001) && (str[i] <= 0x007F)))
                        break;
                    byteArray[count++] = (byte) str[i];
                }
                for (; i < stringLen; i++) {
                    if ((str[i] >= 0x0001) && (str[i] <= 0x007F)) {
                        byteArray[count++] = (byte) str[i];
                    } else if (str[i] > 0x07FF) {
                        byteArray[count++] = (byte) (0xE0 | ((str[i] >> 12) & 0x0F));
                        byteArray[count++] = (byte) (0x80 | ((str[i] >> 6) & 0x3F));
                        byteArray[count++] = (byte) (0x80 | ((str[i]) & 0x3F));
                    } else {
                        byteArray[count++] = (byte) (0xC0 | ((str[i] >> 6) & 0x1F));
                        byteArray[count++] = (byte) (0x80 | ((str[i]) & 0x3F));
                    }
                }
                writeShort(utfLength);
                write(byteArray);
            };

        }
    }
}