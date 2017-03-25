//
//  ItemsDataSource.swift
//  merch-ui
//
//  Created by Rauhul Varma on 3/21/17.
//  Copyright © 2017 acm. All rights reserved.
//

import UIKit

class ItemsDataSource: NSObject, UITableViewDataSource {
    static let shared = ItemsDataSource()
    
    private override init() { }
    
    var items = [ItemModel]()
    
    func clear() {
        items.removeAll(keepingCapacity: true)
    }
    
    func populate(models: [[String: Any]]) {
        for model in models {
            guard let model = ItemModel(json: model) else {
                assertionFailure("Unable to create ItemModel from json")
                continue
            }
            items.append(model)
        }
    }
    
    // MARK: - UITableViewDataSource
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCell(withIdentifier: "ItemCell", for: indexPath)
        
        if let cell = cell as? ItemTableViewCell {
            let item = items[indexPath.row]
            cell.indexPath = indexPath
            cell.costLabel.text = "ℂ\(item.price)"
            cell.titleLabel.text = item.name
            cell.isUserInteractionEnabled = item.quantity > 0
            
            if let image = ImageDownloadController.shared.images.object(forKey: item.imageURL as NSString) {
                cell.iconImageView.image = image
            } else  {
                cell.iconImageView.image = ItemTableViewCell.defaultIconImage
                ImageDownloadController.shared.downloadImage(withURL: item.imageURL, success: { (image) in
                    DispatchQueue.main.async {
                        if cell.indexPath == indexPath {
                            cell.iconImageView?.image = image
                        }
                    }
                }, error: { (error) in
                    print(error)
                })
            }
        }
        
        return cell
    }
    
    func numberOfSections(in tableView: UITableView) -> Int {
        return 1
    }
    
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return items.count
    }
}
