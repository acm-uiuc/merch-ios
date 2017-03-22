//
//  UserModel.swift
//  merch-ui
//
//  Created by Rauhul Varma on 3/19/17.
//  Copyright © 2017 acm. All rights reserved.
//

import Foundation

class UserModel {
    static var shared: UserModel?

    let id: Int
    let netID: String
    let pin: Int
    var balance: Int

    private init(id: Int, netID: String, pin: Int, balance: Int) {
        self.id = id
        self.netID = netID
        self.pin = pin
        self.balance = balance
    }
    
    static func load(json: [String: Any]) {
        guard let id = json["id"] as? Int,
            let netID = json["netid"] as? String,
            let pin = json["pin"] as? Int,
            let balance = json["balance"] as? Int else {
            shared = nil
            return
        }
        
        shared = UserModel(id: id, netID: netID, pin: pin, balance: balance)
    }
}
