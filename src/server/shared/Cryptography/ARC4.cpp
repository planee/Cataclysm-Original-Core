/*
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ARC4.h"
#include <openssl/sha.h>

ARC4::ARC4(uint8 len) : m_ctx()
{
#if defined(OPENSSL_VERSION_NUMBER) && OPENSSL_VERSION_NUMBER >= 0x10100000L
    m_ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(m_ctx);
    EVP_EncryptInit_ex(m_ctx, EVP_rc4(), nullptr, nullptr, nullptr);
    EVP_CIPHER_CTX_set_key_length(m_ctx, len);
#else
    EVP_CIPHER_CTX_init(&m_ctx);
    EVP_EncryptInit_ex(&m_ctx, EVP_rc4(), nullptr, nullptr, nullptr);
    EVP_CIPHER_CTX_set_key_length(&m_ctx, len);
#endif
}

ARC4::ARC4(uint8 *seed, uint8 len) : m_ctx()
{
#if defined(OPENSSL_VERSION_NUMBER) && OPENSSL_VERSION_NUMBER >= 0x10100000L
    m_ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(m_ctx);
    EVP_EncryptInit_ex(m_ctx, EVP_rc4(), nullptr, nullptr, nullptr);
    EVP_CIPHER_CTX_set_key_length(m_ctx, len);
    EVP_EncryptInit_ex(m_ctx, nullptr, nullptr, seed, nullptr);
#else
    EVP_CIPHER_CTX_init(&m_ctx);
    EVP_EncryptInit_ex(&m_ctx, EVP_rc4(), nullptr, nullptr, nullptr);
    EVP_CIPHER_CTX_set_key_length(&m_ctx, len);
    EVP_EncryptInit_ex(&m_ctx, nullptr, nullptr, seed, nullptr);
#endif
}

ARC4::~ARC4()
{
#if defined(OPENSSL_VERSION_NUMBER) && OPENSSL_VERSION_NUMBER >= 0x10100000L
    EVP_CIPHER_CTX_cleanup(m_ctx);
    EVP_CIPHER_CTX_free(m_ctx);
#else
    EVP_CIPHER_CTX_cleanup(&m_ctx);
#endif
}

void ARC4::Init(uint8 *seed)
{
#if defined(OPENSSL_VERSION_NUMBER) && OPENSSL_VERSION_NUMBER >= 0x10100000L
    EVP_EncryptInit_ex(m_ctx, nullptr, nullptr, seed, nullptr);
#else
    EVP_EncryptInit_ex(&m_ctx, nullptr, nullptr, seed, nullptr);
#endif
}

void ARC4::UpdateData(int len, uint8 *data)
{
    int outlen = 0;
#if defined(OPENSSL_VERSION_NUMBER) && OPENSSL_VERSION_NUMBER >= 0x10100000L
    EVP_EncryptUpdate(m_ctx, data, &outlen, data, len);
    EVP_EncryptFinal_ex(m_ctx, data, &outlen);
#else
    EVP_EncryptUpdate(&m_ctx, data, &outlen, data, len);
    EVP_EncryptFinal_ex(&m_ctx, data, &outlen);
#endif
}
