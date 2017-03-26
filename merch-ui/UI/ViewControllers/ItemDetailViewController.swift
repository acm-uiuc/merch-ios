//
//  ItemDetailViewController.swift
//  merch-ui
//
//  Created by Rauhul Varma on 3/25/17.
//  Copyright © 2017 acm. All rights reserved.
//

import UIKit

class ItemDetailViewController: UIViewController {
    static let defaultItemImage = #imageLiteral(resourceName: "default_item_large")
    
    // MARK: - Outlets
    @IBOutlet weak var itemImageView: UIImageView!
    @IBOutlet weak var itemNameLabel: UILabel!
    
    // MARK: - Variables
    var item: ItemModel?
    
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        
        guard let item = item else { return }
        itemNameLabel.text = "Get a \(item.name)?"

        if let image = ImageDownloadController.shared.images.object(forKey: item.imageURL as NSString) {
            itemImageView.image = image
        } else {
            itemImageView.image = ItemDetailViewController.defaultItemImage
            ImageDownloadController.shared.downloadImage(withURL: item.imageURL, success: { (image) in
                DispatchQueue.main.async {
                    self.itemImageView.image = image
                }
            }, error: { (error) in
                print(error)
            })
        }
    }
    
    
    @IBAction func purchase(_ sender: Any?) {
        (parent as? ItemsViewController)?.purchaseItemFailed(withError: "Purchasing Items has not been implemented yet.")
    }
    
    @IBAction func cancelPurchase(_ sender: Any?) {
        (parent as? ItemsViewController)?.cancelledPurchase()
    }

}
