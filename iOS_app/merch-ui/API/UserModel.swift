//
//  UserModel.swift
//  merch-ui
//
//  Created by Rauhul Varma on 3/19/17.
//  Copyright © 2017 acm. All rights reserved.
//

import Foundation

class UserModel {
    static let shared = UserModel()
    
    var netID = ""
    var token = ""
    
    private init() { }
    
    func reset() {
        netID = ""
        token = ""
    }
    
    func load(json: [String: Any]) {
        // TODO: - fill me
    }
}
