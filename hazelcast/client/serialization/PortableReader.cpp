//
//  PortableReader.cpp
//  Server
//
//  Created by sancar koyunlu on 1/10/13.
//  Copyright (c) 2013 sancar koyunlu. All rights reserved.
//
#include "Portable.h"
#include "PortableReader.h"

namespace hazelcast {
    namespace client {
        namespace serialization {

            PortableReader::PortableReader(PortableSerializer *serializer, DataInput& input, boost::shared_ptr<ClassDefinition> cd, Type type)
            : serializer(serializer)
            , input(&input)
            , cd(cd)
            , offset(input.position())
            , type(type)
            , isFieldMorphed(false) {
            };

            int PortableReader::readInt(string fieldName) {
                if (type == MORPHING && !isFieldMorphed) return morphInt(fieldName);
                int pos = getPosition(fieldName);
                return input->readInt(pos);
            };

            long PortableReader::readLong(string fieldName) {
                if (type == MORPHING && !isFieldMorphed) return morphLong(fieldName);
                int pos = getPosition(fieldName);
                return input->readLong(pos);
            };

            bool PortableReader::readBoolean(string fieldName) {
                if (type == MORPHING && !isFieldMorphed) return morphBoolean(fieldName);
                int pos = getPosition(fieldName);
                return input->readBoolean(pos);
            };

            byte PortableReader::readByte(string fieldName) {
                if (type == MORPHING && !isFieldMorphed) return morphByte(fieldName);
                int pos = getPosition(fieldName);
                return input->readByte(pos);
            };

            char PortableReader::readChar(string fieldName) {
                if (type == MORPHING && !isFieldMorphed) return morphChar(fieldName);
                int pos = getPosition(fieldName);
                return input->readChar(pos);
            };

            double PortableReader::readDouble(string fieldName) {
                if (type == MORPHING && !isFieldMorphed) return morphDouble(fieldName);
                int pos = getPosition(fieldName);
                return input->readDouble(pos);
            };

            float PortableReader::readFloat(string fieldName) {
                if (type == MORPHING && !isFieldMorphed) return morphFloat(fieldName);
                int pos = getPosition(fieldName);
                return input->readFloat(pos);
            };

            short PortableReader::readShort(string fieldName) {
                if (type == MORPHING && !isFieldMorphed) return morphShort(fieldName);
                int pos = getPosition(fieldName);
                return input->readShort(pos);
            };

            string PortableReader::readUTF(string fieldName) {
                if (type == MORPHING && !isFieldMorphed) return morphUTF(fieldName);
                int pos = getPosition(fieldName);
                return input->readUTF(pos);
            };

            std::vector<byte> PortableReader::readByteArray(string fieldName) {
                if (type == MORPHING && !isFieldMorphed) return morphByteArray(fieldName);
                int pos = getPosition(fieldName);
                input->position(pos);
                int len = input->readInt();
                std::vector<byte> values(len);
                for (int i = 0; i < len; i++) {
                    values[i] = input->readByte();
                }
                return values;
            };

            std::vector<char> PortableReader::readCharArray(string fieldName) {
                if (type == MORPHING && !isFieldMorphed) return morphCharArray(fieldName);
                int pos = getPosition(fieldName);
                input->position(pos);
                int len = input->readInt();
                std::vector<char> values(len);
                for (int i = 0; i < len; i++) {
                    values[i] = input->readChar();
                }
                return values;
            };

            std::vector<int> PortableReader::readIntArray(string fieldName) {
                if (type == MORPHING && !isFieldMorphed) return morphIntArray(fieldName);
                int pos = getPosition(fieldName);
                input->position(pos);
                int len = input->readInt();
                std::vector<int> values(len);
                for (int i = 0; i < len; i++) {
                    values[i] = input->readInt();
                }
                return values;
            };

            std::vector<long> PortableReader::readLongArray(string fieldName) {
                if (type == MORPHING && !isFieldMorphed) return morphLongArray(fieldName);
                int pos = getPosition(fieldName);
                input->position(pos);
                int len = input->readInt();
                std::vector<long> values(len);
                for (int i = 0; i < len; i++) {
                    values[i] = input->readLong();
                }
                return values;
            };

            std::vector<double> PortableReader::readDoubleArray(string fieldName) {
                if (type == MORPHING && !isFieldMorphed) return morphDoubleArray(fieldName);
                int pos = getPosition(fieldName);
                input->position(pos);
                int len = input->readInt();
                std::vector<double> values(len);
                for (int i = 0; i < len; i++) {
                    values[i] = input->readDouble();
                }
                return values;
            };

            std::vector<float> PortableReader::readFloatArray(string fieldName) {
                if (type == MORPHING && !isFieldMorphed) return morphFloatArray(fieldName);
                int pos = getPosition(fieldName);
                input->position(pos);
                int len = input->readInt();
                std::vector<float> values(len);
                for (int i = 0; i < len; i++) {
                    values[i] = input->readFloat();
                }
                return values;
            };

            std::vector<short> PortableReader::readShortArray(string fieldName) {
                if (type == MORPHING && !isFieldMorphed) return morphShortArray(fieldName);
                int pos = getPosition(fieldName);
                input->position(pos);
                int len = input->readInt();
                std::vector<short> values(len);
                for (int i = 0; i < len; i++) {
                    values[i] = input->readShort();
                }
                return values;
            };

            DataInput *PortableReader::getRawDataInput() {
                return input;
            }

            int PortableReader::getPosition(string fieldName) {
                isFieldMorphed = false;
                if (!cd->isFieldDefinitionExists(fieldName))
                    throw hazelcast::client::HazelcastException("PortableReader::getPosition : unknownField " + fieldName);
                FieldDefinition fd = cd->get(fieldName);
                return getPosition(&fd);
            };

            int PortableReader::getPosition(FieldDefinition *fd) {
                return input->readInt(offset + fd->getIndex() * sizeof (int));
            };

            int PortableReader::morphInt(string fieldName) {
                isFieldMorphed = true;
                if (!cd->isFieldDefinitionExists(fieldName))
                    return 0;
                FieldDefinition fd = cd->get(fieldName);

                FieldType type = fd.getType();
                if (type == FieldTypes::TYPE_INT) {
                    return readInt(fieldName);
                } else if (type == FieldTypes::TYPE_BYTE) {
                    return readByte(fieldName);
                } else if (type == FieldTypes::TYPE_CHAR) {
                    return readChar(fieldName);
                } else if (type == FieldTypes::TYPE_SHORT) {
                    return readShort(fieldName);
                } else {
                    throw hazelcast::client::HazelcastException("IncompatibleClassChangeError");
                }
            };

            long PortableReader::morphLong(string fieldName) {
                isFieldMorphed = true;
                if (!cd->isFieldDefinitionExists(fieldName))
                    return 0;
                FieldDefinition fd = cd->get(fieldName);

                FieldType type = fd.getType();
                if (type == FieldTypes::TYPE_LONG) {
                    return readLong(fieldName);
                } else if (type == FieldTypes::TYPE_INT) {
                    return readInt(fieldName);
                } else if (type == FieldTypes::TYPE_BYTE) {
                    return readByte(fieldName);
                } else if (type == FieldTypes::TYPE_CHAR) {
                    return readChar(fieldName);
                } else if (type == FieldTypes::TYPE_SHORT) {
                    return readShort(fieldName);
                } else {
                    throw hazelcast::client::HazelcastException("IncompatibleClassChangeError");
                }
            };

            bool PortableReader::morphBoolean(string fieldName) {
                isFieldMorphed = true;
                if (!cd->isFieldDefinitionExists(fieldName))
                    return 0;
                FieldDefinition fd = cd->get(fieldName);

                if (fd.getType() != FieldTypes::TYPE_BOOLEAN)
                    throw hazelcast::client::HazelcastException("IncompatibleClassChangeError");

                return readBoolean(fieldName);
            };

            byte PortableReader::morphByte(string fieldName) {
                isFieldMorphed = true;
                if (!cd->isFieldDefinitionExists(fieldName))
                    return 0;
                FieldDefinition fd = cd->get(fieldName);

                if (fd.getType() != FieldTypes::TYPE_BYTE)
                    throw hazelcast::client::HazelcastException("IncompatibleClassChangeError");

                return readByte(fieldName);
            };

            char PortableReader::morphChar(string fieldName) {
                isFieldMorphed = true;
                if (!cd->isFieldDefinitionExists(fieldName))
                    return 0;
                FieldDefinition fd = cd->get(fieldName);

                if (fd.getType() != FieldTypes::TYPE_CHAR)
                    throw hazelcast::client::HazelcastException("IncompatibleClassChangeError");

                return readChar(fieldName);
            };

            double PortableReader::morphDouble(string fieldName) {
                isFieldMorphed = true;
                if (!cd->isFieldDefinitionExists(fieldName))
                    return 0;
                FieldDefinition fd = cd->get(fieldName);

                FieldType type = fd.getType();
                if (type == FieldTypes::TYPE_FLOAT) {
                    return readFloat(fieldName);
                } else if (type == FieldTypes::TYPE_DOUBLE) {
                    return readDouble(fieldName);
                } else if (type == FieldTypes::TYPE_LONG) {
                    return readLong(fieldName);
                } else if (type == FieldTypes::TYPE_INT) {
                    return readInt(fieldName);
                } else if (type == FieldTypes::TYPE_BYTE) {
                    return readByte(fieldName);
                } else if (type == FieldTypes::TYPE_CHAR) {
                    return readChar(fieldName);
                } else if (type == FieldTypes::TYPE_SHORT) {
                    return readShort(fieldName);
                } else {
                    throw hazelcast::client::HazelcastException("IncompatibleClassChangeError");
                }
            };

            float PortableReader::morphFloat(string fieldName) {
                isFieldMorphed = true;
                if (!cd->isFieldDefinitionExists(fieldName))
                    return 0;
                FieldDefinition fd = cd->get(fieldName);

                FieldType type = fd.getType();
                if (type == FieldTypes::TYPE_FLOAT) {
                    return readFloat(fieldName);
                } else if (type == FieldTypes::TYPE_INT) {
                    return readInt(fieldName);
                } else if (type == FieldTypes::TYPE_BYTE) {
                    return readByte(fieldName);
                } else if (type == FieldTypes::TYPE_CHAR) {
                    return readChar(fieldName);
                } else if (type == FieldTypes::TYPE_SHORT) {
                    return readShort(fieldName);
                } else {
                    throw hazelcast::client::HazelcastException("IncompatibleClassChangeError");
                }
            };

            short PortableReader::morphShort(string fieldName) {
                isFieldMorphed = true;
                if (!cd->isFieldDefinitionExists(fieldName))
                    return 0;
                FieldDefinition fd = cd->get(fieldName);

                FieldType type = fd.getType();
                if (type == FieldTypes::TYPE_BYTE) {
                    return readByte(fieldName);
                }
                if (type == FieldTypes::TYPE_SHORT) {
                    return readShort(fieldName);
                } else {
                    throw hazelcast::client::HazelcastException("IncompatibleClassChangeError");
                }
            };

            string PortableReader::morphUTF(string fieldName) {
                isFieldMorphed = true;
                if (!cd->isFieldDefinitionExists(fieldName))
                    return NULL;
                FieldDefinition fd = cd->get(fieldName);

                if (fd.getType() != FieldTypes::TYPE_UTF) {
                    throw hazelcast::client::HazelcastException("IncompatibleClassChangeError");
                }
                return readUTF(fieldName);
            };

            std::vector<byte> PortableReader::morphByteArray(string fieldName) {
                isFieldMorphed = true;
                if (!cd->isFieldDefinitionExists(fieldName))
                    throw "throwUnknownFieldException" + fieldName;
                FieldDefinition fd = cd->get(fieldName);

                if (fd.getType() != FieldTypes::TYPE_BYTE_ARRAY) {
                    throw hazelcast::client::HazelcastException("IncompatibleClassChangeError");
                }
                return readByteArray(fieldName);
            };

            std::vector<char> PortableReader::morphCharArray(string fieldName) {
                isFieldMorphed = true;
                if (!cd->isFieldDefinitionExists(fieldName))
                    throw "throwUnknownFieldException" + fieldName;
                FieldDefinition fd = cd->get(fieldName);

                if (fd.getType() != FieldTypes::TYPE_CHAR_ARRAY) {
                    throw hazelcast::client::HazelcastException("IncompatibleClassChangeError");
                }
                return readCharArray(fieldName);
            };

            std::vector<int> PortableReader::morphIntArray(string fieldName) {
                isFieldMorphed = true;
                if (!cd->isFieldDefinitionExists(fieldName))
                    throw "throwUnknownFieldException" + fieldName;
                FieldDefinition fd = cd->get(fieldName);

                if (fd.getType() != FieldTypes::TYPE_INT_ARRAY) {
                    throw hazelcast::client::HazelcastException("IncompatibleClassChangeError");
                }
                return readIntArray(fieldName);
            };

            std::vector<long> PortableReader::morphLongArray(string fieldName) {
                isFieldMorphed = true;
                if (!cd->isFieldDefinitionExists(fieldName))
                    throw "throwUnknownFieldException" + fieldName;
                FieldDefinition fd = cd->get(fieldName);

                if (fd.getType() != FieldTypes::TYPE_LONG_ARRAY) {
                    throw hazelcast::client::HazelcastException("IncompatibleClassChangeError");
                }
                return readLongArray(fieldName);
            };

            std::vector<double> PortableReader::morphDoubleArray(string fieldName) {
                isFieldMorphed = true;
                if (!cd->isFieldDefinitionExists(fieldName))
                    throw "throwUnknownFieldException" + fieldName;
                FieldDefinition fd = cd->get(fieldName);

                if (fd.getType() != FieldTypes::TYPE_DOUBLE_ARRAY) {
                    throw hazelcast::client::HazelcastException("IncompatibleClassChangeError");
                }
                return readDoubleArray(fieldName);
            };

            std::vector<float> PortableReader::morphFloatArray(string fieldName) {
                isFieldMorphed = true;
                if (!cd->isFieldDefinitionExists(fieldName))
                    throw "throwUnknownFieldException" + fieldName;
                FieldDefinition fd = cd->get(fieldName);

                if (fd.getType() != FieldTypes::TYPE_FLOAT_ARRAY) {
                    throw hazelcast::client::HazelcastException("IncompatibleClassChangeError");
                }
                return readFloatArray(fieldName);
            };

            std::vector<short> PortableReader::morphShortArray(string fieldName) {
                isFieldMorphed = true;
                if (!cd->isFieldDefinitionExists(fieldName))
                    throw "throwUnknownFieldException" + fieldName;
                FieldDefinition fd = cd->get(fieldName);

                if (fd.getType() != FieldTypes::TYPE_SHORT_ARRAY) {
                    throw hazelcast::client::HazelcastException("IncompatibleClassChangeError");
                }
                return readShortArray(fieldName);
            };

        }
    }
}
