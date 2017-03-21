//
//  MerchUIElements.swift
//  merch-ui
//
//  Created by Rauhul Varma on 3/10/17.
//  Copyright © 2017 acm. All rights reserved.
//

import UIKit

class MUIHidingLabel: UILabel {
    override var text: String? {
        get {
            return super.text
        }
        set {
            if let newValue = newValue {
                super.text = newValue
                if alpha != 1 {
                    UIView.animate(withDuration: 0.5) {
                        self.alpha = 1
                    }
                }
            } else {
                if alpha != 0 {
                    UIView.animate(withDuration: 0.5, animations: { 
                        self.alpha = 0
                    }) { (_) in
                        super.text = nil
                    }
                } else {
                    super.text = nil
                }
            }
        }
    }
}
