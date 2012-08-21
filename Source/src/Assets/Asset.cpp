/**
 * @file
 *	Implementation of the IAsset interface.
 *	Though it's referred to as an "interface," it's more of just a base
 *	class because some things are defined by default.
 *
 * @author  George Kudrayvtsev
 * @version 1.0
 */

#include "Assets/Asset.h"

using asset::IAsset;

IAsset::IAsset() : m_id(CURRENT_ASSET_ID),
    m_loaded(false),
    m_refcount(1)
{
    // Increase asset id.
    // This number also represents the total amount of assets loaded.
    asset::CURRENT_ASSET_ID++;
}

IAsset::~IAsset()
{
    m_refcount--;
    m_loaded = false;
}

const std::string& IAsset::GetFilename() const
{
    return m_filename;
}

unsigned int IAsset::GetReferenceCount() const
{
    return m_refcount;
}

asset::asset_id IAsset::GetID() const
{
    return m_id;
}

void asset::IAsset::IncrementReferenceCount()
{
    m_refcount++;
}
