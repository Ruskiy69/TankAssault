/**
 * @file
 *  Implementation of the CAsset interface.
 *  Though it's referred to as an "interface," it's more of just a base
 *  class because some things are defined by default.
 *
 * @author  George Kudrayvtsev
 * @version 1.0
 **/

#include "Assets/Asset.hpp"

using asset::CAsset;

CAsset::CAsset() : m_id(CAsset::Hash((char*)this, sizeof(CAsset))),
    m_loaded(false)
{
    // Increase the total amount of assets loaded.
    ++asset::ASSET_COUNT;
}

CAsset::~CAsset()
{
    m_loaded = false;
}

const char* CAsset::GetFilename() const
{
    return m_filename.c_str();
}

asset::asset_id CAsset::GetID() const
{
    return m_id;
}
