// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*!
 * @file AESGCMGMAC_KeyExchange.cpp
 */

#include <sstream>
#include <fastrtps/rtps/common/Token.h>
#include <fastrtps/rtps/common/BinaryProperty.h>
#include "AESGCMGMAC_KeyExchange.h"

#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

using namespace eprosima::fastrtps::rtps::security;

AESGCMGMAC_KeyExchange::AESGCMGMAC_KeyExchange(){}
AESGCMGMAC_KeyExchange::~AESGCMGMAC_KeyExchange(){}

bool AESGCMGMAC_KeyExchange::create_local_participant_crypto_tokens(
            ParticipantCryptoTokenSeq &local_participant_crypto_tokens,
            ParticipantCryptoHandle &local_participant_crypto,
            ParticipantCryptoHandle &remote_participant_crypto,
            SecurityException &exception){
    
    AESGCMGMAC_ParticipantCryptoHandle& remote_participant = AESGCMGMAC_ParticipantCryptoHandle::narrow(remote_participant_crypto);
    
    //ParticipantKeyMaterial
    {
        ParticipantCryptoToken temp;
        temp.class_id() = std::string("DDS:Crypto:AES_GCM_GMAC");
        BinaryProperty prop;
        prop.name() = std::string("dds.cryp.keymat");
        std::vector<uint8_t> plaintext= KeyMaterialCDRSerialize(remote_participant->Participant2ParticipantKeyMaterial.at(0));
        prop.value() = aes_128_gcm_encrypt(plaintext, remote_participant->master_sender_key); 

        temp.binary_properties().push_back(prop);
        local_participant_crypto_tokens.push_back(temp);
    }

    exception = SecurityException("Not implemented"); 
    return false;
}
     
bool AESGCMGMAC_KeyExchange::set_remote_participant_crypto_tokens(
            const ParticipantCryptoHandle &local_participant_crypto,
            const ParticipantCryptoHandle &remote_participant_crypto,
            const ParticipantCryptoTokenSeq &remote_participant_tokens,
            SecurityException &exception){

    exception = SecurityException("Not implemented"); 
    return false;
}

bool AESGCMGMAC_KeyExchange::create_local_datawriter_crypto_tokens(
            DatawriterCryptoTokenSeq &local_datawriter_crypto_tokens,
            const DatawriterCryptoHandle &local_datawriter_crypto,
            const DatareaderCryptoHandle &remote_datareader_crypto,
            SecurityException &exception){

    exception = SecurityException("Not implemented"); 
    return false;
}

bool AESGCMGMAC_KeyExchange::create_local_datareader_crypto_tokens(
            DatareaderCryptoTokenSeq &local_datareader_crypto_tokens,
            const DatareaderCryptoHandle &local_datareader_crypto,
            const DatawriterCryptoHandle &remote_datawriter_crypto,
            SecurityException &exception){

    exception = SecurityException("Not implemented"); 
    return false;
}

bool AESGCMGMAC_KeyExchange::set_remote_datareader_crypto_tokens(
            const DatawriterCryptoHandle &local_datawriter_crypto,
            const DatareaderCryptoHandle &remote_datareader_crypto,
            const DatareaderCryptoTokenSeq &remote_datareader_tokens,
            SecurityException &exception){

    exception = SecurityException("Not implemented"); 
    return false;
}

bool AESGCMGMAC_KeyExchange::return_crypto_tokens(
            const CryptoTokenSeq &crypto_tokens,
            SecurityException &exception){

    exception = SecurityException("Not implemented"); 
    return false;
}

std::vector<uint8_t> AESGCMGMAC_KeyExchange::KeyMaterialCDRSerialize(KeyMaterial_AES_GCM_GMAC *key){

std::vector<uint8_t> buffer;
    
    buffer.push_back(4);
    for(int i=0;i<4;i++){
        buffer.push_back(key->transformation_kind[i]);
    }
    buffer.push_back(2);
    for(int i=0;i<32;i++){
        buffer.push_back(key->master_salt[i]);
    }
    buffer.push_back(4);
    for(int i=0;i<4;i++){
        buffer.push_back(key->sender_key_id[i]);
    }
    buffer.push_back(32);
    for(int i=0;i<32;i++){
        buffer.push_back(key->master_sender_key[i]);
    }
    buffer.push_back(4);
    for(int i=0;i<4;i++){
        buffer.push_back(key->receiver_specific_key_id[i]);
    }
    buffer.push_back(32);
    for(int i=0;i<32;i++){
        buffer.push_back(key->master_receiver_specific_key[i]);
    }

    return buffer;
}

KeyMaterial_AES_GCM_GMAC AESGCMGMAC_KeyExchange::KeyMaterialCDRDeserialize(std::vector<uint8_t> *CDR){
unsigned short index = 0;
KeyMaterial_AES_GCM_GMAC buffer;
    for(int i=1; i<5; i++){
        buffer.transformation_kind[i-1] = CDR->at(i);
    }
    for(int i=6; i<38; i++){
        buffer.master_salt[i-6] = CDR->at(i);
    }
    for(int i=39; i<43; i++){
        buffer.sender_key_id[i-39] = CDR->at(i);
    }
    for(int i=44; i<76; i++){
        buffer.master_sender_key[i-44] = CDR->at(i);
    }
    for(int i=77; i<81; i++){
        buffer.receiver_specific_key_id[i-77] = CDR->at(i);
    }
    for(int i=82; i< 114; i++){
        buffer.master_receiver_specific_key[i-82] = CDR->at(i);
    }
    return buffer;
}

std::vector<uint8_t> AESGCMGMAC_KeyExchange::aes_128_gcm_encrypt(std::vector<uint8_t> plaintext, std::string key){
    
    OpenSSL_add_all_ciphers();
    int rv = RAND_load_file("/dev/urandom", 32); //Init random number gen

    size_t enc_length = plaintext.length()*3;
    std::vector<uint8_t> output;
    output.resize(enc_length,'\0');

    unsigned char tag[AES_BLOCK_SIZE];
    unsigned char iv[AES_BLOCK_SIZE];
    RAND_bytes(iv, sizeof(iv));
    std::copy(iv, iv+16, output.begin()+16);
    
    int actual_size=0, final_size=0;
    EVP_CIPHER_CTX* e_ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit(e_ctx, EVP_aes_128_gcm(), (const unsigned char*)key.c_str(), iv);
    EVP_EncryptUpdate(e_ctx, &output[32], &actual_size, (const unsigned char*)plaintext.data(), plaintext.length());
    EVP_EncryptFinal(e_ctx, &output[32+actual_size], &final_size);
    EVP_CIPHER_CTX_ctrl(e_ctx, EVP_CTRL_GCM_GET_TAG, 16, tag);
    std::copy(iv,iv+16, output.begin());
    std::copy(iv, iv+16, output.begin()+16);
    output.resize(32+actual_size+final_size);
    EVP_CIPHER_CTX_free(e_ctx);

    return output;
}