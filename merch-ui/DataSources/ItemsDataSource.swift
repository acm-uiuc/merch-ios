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
    
    var models = [ItemModel]()
    
    func clear() {
        models.removeAll(keepingCapacity: true)
    }
    
    func populate(models json: [String: Any]) {
        guard let modelDicts = json["data"] as? [[String: Any]] else {
            assertionFailure("Unable to extract data from json")
            return
        }
        
        for modelDict in modelDicts {
            guard let model = ItemModel(json: modelDict) else {
                assertionFailure("Unable to create ItemModel from json")
                continue
            }
            models.append(model)
        }
    }
    
    // MARK: - UITableViewDataSource
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        return tableView.dequeueReusableCell(withIdentifier: "ItemCell", for: indexPath)
    }
    
    func numberOfSections(in tableView: UITableView) -> Int {
        return 1
    }
    
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return 5
    }
}
