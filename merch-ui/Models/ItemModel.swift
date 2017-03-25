//
//  ItemModel.swift
//  merch-ui
//
//  Created by Rauhul Varma on 3/21/17.
//  Copyright © 2017 acm. All rights reserved.
//

import Foundation

class ItemModel {
    
    let id: Int
    let price: Int
    let name: String
    let imageURL: String
    let quantity: Int
    
    init?(json: [String: Any]) {
        guard let id = json["id"] as? Int,
            let price = json["price"] as? Int,
            let name = json["name"] as? String,
            let imageURL = json["image"] as? String,
            let quantity = json["quantity"] as? Int else { return nil }
        
        self.id = id
        self.price = price
        self.name = name
        self.imageURL = imageURL
        self.quantity = quantity
        
        // TODO: Get image
    }
    
    
}
