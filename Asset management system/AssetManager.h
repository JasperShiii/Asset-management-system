#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

// Supported asset types
enum class AssetType {Texture, Audio, Model };

// Metadata for an asset
struct AssetMetadata {
    std::string name;
    std::string path;
    AssetType type;
    std::vector<std::string> keywords;
    std::string category;
    int version;

    AssetMetadata(std::string name, std::string path, AssetType type, std::vector<std::string> keywords, std::string category, int version)
        : name(std::move(name)), path(std::move(path)), type(type), keywords(std::move(keywords)), category(std::move(category)), version(version) {}

};

// Database of assets
class AssetDatabase {
private:
    std::unordered_map<std::string, AssetMetadata> assets;

public:
    void AddAsset(const AssetMetadata& asset);
    void RemoveAsset(const std::string& name);
    AssetMetadata GetAsset(const std::string& name) const;
    std::vector<AssetMetadata> GetAssetsByType(const AssetType& type) const;
    std::vector<AssetMetadata> GetAssetsByKeyword(const std::string& keyword) const;
};


class Asset {
public:
    Asset(const AssetMetadata& metadata) : metadata_(metadata) {}
    const AssetMetadata& GetMetadata() const { return metadata_; }
    void AddRelatedAsset(const Asset* asset) { relatedAssets_.push_back(asset); }
    const std::vector<const Asset*>& GetRelatedAssets() const { return relatedAssets_; }
    // add other Asset functionality as needed

private:
    AssetMetadata metadata_;
    std::vector<const Asset*> relatedAssets_;
};


class AssetManager {
public:
    void AddAsset(const AssetMetadata& metadata);
    void RemoveAsset(const std::string& name);
    std::vector<const Asset*> GetAssetsByType(AssetType type) const;
    std::vector<const Asset*> GetAssetsByKeyword(const std::string& keyword) const;
    std::vector<const Asset*> GetRelatedAssets(const std::string& name) const;
    const Asset* GetAssetByName(const std::string& name) const {
        for (const auto& asset : assets_) {
            if (asset->GetMetadata().name == name) {
                return asset.get();
            }
        }
        return nullptr;
    }

private:
    std::vector<std::unique_ptr<Asset>> assets_;
    std::unordered_map<std::string, std::vector<Asset*>> assetsByKeyword_;
};

std::vector<const Asset*> AssetManager::GetRelatedAssets(const std::string& name) const {
    std::vector<const Asset*> relatedAssets;

    const Asset* asset = GetAssetByName(name);
    if (!asset) {
        return relatedAssets;
    }

    for (const auto& related : asset->GetRelatedAssets()) {
        relatedAssets.push_back(related);
    }

    return relatedAssets;
}


#endif 
