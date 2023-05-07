#include <iostream>
#include "AssetManager.h"

// AssetDatabase implementation

void AssetDatabase::AddAsset(const AssetMetadata& asset) {
    assets.emplace(asset.name, asset);
}

void AssetDatabase::RemoveAsset(const std::string& name) {
    assets.erase(name);
}

AssetMetadata AssetDatabase::GetAsset(const std::string& name) const {
    auto it = assets.find(name);
    if (it != assets.end()) {
        return it->second;
    }
    else {
        return AssetMetadata("", "", AssetType::Texture, {}, "", 0);
    }
}

std::vector<AssetMetadata> AssetDatabase::GetAssetsByType(const AssetType& type) const {
    std::vector<AssetMetadata> assetsOfType;
    for (const auto& asset : assets) {
        if (asset.second.type == type) {
            assetsOfType.push_back(asset.second);
        }
    }
    return assetsOfType;
}

std::vector<AssetMetadata> AssetDatabase::GetAssetsByKeyword(const std::string& keyword) const {
    std::vector<AssetMetadata> assetsWithKeyword;
    for (const auto& asset : assets) {
        if (std::find(asset.second.keywords.begin(), asset.second.keywords.end(), keyword) != asset.second.keywords.end()) {
            assetsWithKeyword.push_back(asset.second);
        }
    }
    return assetsWithKeyword;
}

// AssetManager implementation

void AssetManager::AddAsset(const AssetMetadata& metadata) {
    auto asset = std::make_unique<Asset>(metadata);
    assets_.push_back(std::move(asset));
    for (const auto& keyword : metadata.keywords) {
        assetsByKeyword_[keyword].push_back(assets_.back().get());
    }
}


void AssetManager::RemoveAsset(const std::string& name) {
    for (auto it = assets_.begin(); it != assets_.end(); ++it) {
        if ((*it)->GetMetadata().name == name) {
            const auto& keywords = (*it)->GetMetadata().keywords;
            for (const auto& keyword : keywords) {
                auto& assetList = assetsByKeyword_[keyword];
                assetList.erase(std::remove(assetList.begin(), assetList.end(), it->get()), assetList.end());
            }
            assets_.erase(it);
            return;
        }
    }
}

std::vector<const Asset*> AssetManager::GetAssetsByType(AssetType type) const {
    std::vector<const Asset*> assetsOfType;
    for (const auto& asset : assets_) {
        if (asset->GetMetadata().type == type) {
            assetsOfType.push_back(asset.get());
        }
    }
    return assetsOfType;
}

std::vector<const Asset*> AssetManager::GetAssetsByKeyword(const std::string& keyword) const {
    const auto it = assetsByKeyword_.find(keyword);
    if (it != assetsByKeyword_.end()) {
        return std::vector<const Asset*>(it->second.begin(), it->second.end());
    }
    return std::vector<const Asset*>();
}


int main() {
    AssetMetadata textureMetadata("Texture1", "Textures/Texture1.png", AssetType::Texture, { "background", "game" }, "Game Assets", 1);
    AssetMetadata audioMetadata("Audio1", "Audio/Audio1.wav", AssetType::Audio, { "background", "game" }, "Game Assets", 1);
    AssetMetadata modelMetadata("Model1", "Models/Model1.obj", AssetType::Model, { "3d", "game" }, "Game Assets", 1);

    AssetManager assetManager;

    assetManager.AddAsset(textureMetadata);
    assetManager.AddAsset(audioMetadata);
    assetManager.AddAsset(modelMetadata);

    auto assetsWithType = assetManager.GetAssetsByType(AssetType::Texture);
    for (const auto& asset : assetsWithType) {
        std::cout << asset->GetMetadata().name << " - " << asset->GetMetadata().path << std::endl;
    }

    auto assetsWithKeyword = assetManager.GetAssetsByKeyword("game");
    for (const auto& asset : assetsWithKeyword) {
        std::cout << asset->GetMetadata().name << " - " << asset->GetMetadata().path << std::endl;
    }


    while (true) {
        std::cout << "Asset Manager" << std::endl;
        std::cout << "1. Add Asset" << std::endl;
        std::cout << "2. List Assets by Type" << std::endl;
        std::cout << "3. List Assets by Keyword" << std::endl;
        std::cout << "4. Remove Asset" << std::endl;
        std::cout << "5. Enter an asset name to query the associated assets" << std::endl;
        std::cout << "6. Exit" << std::endl;

        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1: {
            std::cout << "Enter Asset Name: ";
            std::string name;
            std::cin >> name;

            std::cout << "Enter Asset Path: ";
            std::string path;
            std::cin >> path;

            std::cout << "Enter Asset Type (0 - Texture, 1 - Audio, 2 - Model): ";
            int type;
            std::cin >> type;

            std::cout << "Enter Asset Keyword: ";
            std::string keyword;
            std::cin >> keyword;

            std::cout << "Enter Asset Category: ";
            std::string category;
            std::cin >> category;

            std::cout << "Enter Asset Version: ";
            int version;
            std::cin >> version;

            AssetMetadata metadata(name, path, static_cast<AssetType>(type), std::vector<std::string>{ keyword }, category, version);
            assetManager.AddAsset(metadata);

            break;
        }

        case 2: {
            std::cout << "Enter Asset Type (0 - Texture, 1 - Audio, 2 - Model): ";
            int type;
            std::cin >> type;

            auto assetsWithType = assetManager.GetAssetsByType(static_cast<AssetType>(type));
            for (const auto& asset : assetsWithType) {
                std::cout << asset->GetMetadata().name << " - " << asset->GetMetadata().path << std::endl;
            }

            break;
        }

        case 3: {
            std::cout << "Enter Asset Keyword: ";
            std::string keyword;
            std::cin >> keyword;

            auto assetsWithKeyword = assetManager.GetAssetsByKeyword(keyword);
            for (const auto& asset : assetsWithKeyword) {
                std::cout << asset->GetMetadata().name << " - " << asset->GetMetadata().path << std::endl;
            }

            break;
        }

        case 4: {
            std::cout << "Enter Asset Name: ";
            std::string name;
            std::cin >> name;

            assetManager.RemoveAsset(name);

            break;
        }

        case 5: {
            std::cout << "Enter Asset Name: ";
            std::string name;
            std::cin >> name;

            auto relatedAssets = assetManager.GetRelatedAssets(name);
            if (relatedAssets.empty()) {
                std::cout << "No related assets found." << std::endl;
            }
            else {
                std::cout << "Related assets:" << std::endl;
                for (const auto& asset : relatedAssets) {
                    std::cout << asset->GetMetadata().name << " - " << asset->GetMetadata().path << std::endl;
                }
            }

            break;
        }

        case 6: {

            return 0;
        }

        }

    }
}
    