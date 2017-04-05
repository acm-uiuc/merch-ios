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
    let total_stock: Int
    
    init?(json: [String: Any]) {
        guard let id = json["id"] as? Int,
            let price = json["price"] as? Int,
            let name = json["name"] as? String,
            let imageURL = json["image"] as? String,
            let total_stock = json["total_stock"] as? Int else { return nil }
        
        self.id = id
        self.price = price
        self.name = name
        self.imageURL = imageURL
        self.total_stock = total_stock
        
    }
    
    
}
